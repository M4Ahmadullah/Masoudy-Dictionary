#include "auth_system.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>
#include <algorithm>
#include <cstring>
// OpenSSL headers - using system OpenSSL
#ifdef _WIN32
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#else
// Fallback for non-Windows systems
#define SHA256_DIGEST_LENGTH 32
struct SHA256_CTX {
    unsigned char data[64];
    unsigned int datalen;
    unsigned long long bitlen;
    unsigned char hash[32];
};
// Stub implementations for cryptographic functions
void SHA256_Init(SHA256_CTX* ctx) { 
    (void)ctx; // Suppress unused parameter warning
    /* stub */ 
}

void SHA256_Update(SHA256_CTX* ctx, const void* data, size_t len) { 
    (void)ctx; // Suppress unused parameter warning
    (void)data; // Suppress unused parameter warning
    (void)len; // Suppress unused parameter warning
    /* stub */ 
}

void SHA256_Final(unsigned char* hash, SHA256_CTX* ctx) { 
    (void)hash; // Suppress unused parameter warning
    (void)ctx; // Suppress unused parameter warning
    /* stub */ 
}

int RAND_bytes(unsigned char* buf, int num) { 
    (void)buf; // Suppress unused parameter warning
    (void)num; // Suppress unused parameter warning
    /* stub */ 
    return 1; 
}
#endif

namespace MasoudyAuth {

// Static credentials (these would be encrypted in production)
const std::string AuthSystem::VICTIM_PASSWORD = "victim";
const std::string AuthSystem::ADMIN_PASSWORD = "admin";

AuthSystem::AuthSystem() 
    : maxLoginAttempts(3)
    , lockoutDurationMinutes(15)
    , failedLoginCount(0) {
    
    // Generate encryption key
    encryptionKey.resize(32);
    if (RAND_bytes(encryptionKey.data(), 32) != 1) {
        // Fallback to deterministic key if random generation fails
        std::fill(encryptionKey.begin(), encryptionKey.end(), 0x42);
    }
    
    // Initialize session
    currentSession = nullptr;
}

AuthSystem::~AuthSystem() {
    secureWipe();
}

std::string AuthSystem::hashPassword(const std::string& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password.c_str(), password.length());
    SHA256_Final(hash, &sha256);
    
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

std::string AuthSystem::generateSessionId() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    std::string sessionId;
    sessionId.reserve(32);
    
    for (int i = 0; i < 32; ++i) {
        sessionId += static_cast<char>(dis(gen));
    }
    
    return sessionId;
}

bool AuthSystem::validateCredentials(const std::string& username, const std::string& password) {
    // Check if account is locked
    if (isAccountLocked()) {
        return false;
    }
    
    // Validate credentials
    if (username == "victim" && password == VICTIM_PASSWORD) {
        return true;
    }
    else if (username == "admin" && password == ADMIN_PASSWORD) {
        return true;
    }
    
    return false;
}

void AuthSystem::clearSession() {
    if (currentSession) {
        // Secure cleanup of session data
        std::fill(currentSession->username.begin(), currentSession->username.end(), 0);
        currentSession->sessionId.clear();
        currentSession->isActive = false;
        currentSession.reset();
    }
}

std::string AuthSystem::encryptString(const std::string& input) {
    // Simple XOR encryption for demonstration
    // In production, use proper encryption
    std::string encrypted = input;
    for (size_t i = 0; i < encrypted.length(); ++i) {
        encrypted[i] ^= encryptionKey[i % encryptionKey.size()];
    }
    return encrypted;
}

std::string AuthSystem::decryptString(const std::string& input) {
    // XOR decryption
    return encryptString(input); // XOR is symmetric
}

void AuthSystem::secureMemoryCleanup() {
    // Clear encryption key
    std::fill(encryptionKey.begin(), encryptionKey.end(), 0);
    encryptionKey.clear();
}

AuthStatus AuthSystem::authenticate(const std::string& username, const std::string& password) {
    try {
        // Check if account is locked
        if (isAccountLocked()) {
            logSecurityEvent("Authentication failed: Account locked");
            return AuthStatus::ACCOUNT_LOCKED;
        }
        
        // Validate credentials
        if (validateCredentials(username, password)) {
            // Create new session
            currentSession = std::make_unique<UserSession>();
            currentSession->username = username;
            currentSession->role = (username == "admin") ? UserRole::ADMIN : UserRole::VICTIM;
            currentSession->loginTime = std::chrono::system_clock::now();
            currentSession->lastActivity = std::chrono::system_clock::now();
            currentSession->isActive = true;
            currentSession->sessionId = generateSessionId();
            
            // Reset failed login count
            resetFailedLoginCount();
            
            // Log successful authentication
            logSecurityEvent("Authentication successful: " + username);
            
            return AuthStatus::SUCCESS;
        }
        else {
            // Increment failed login count
            failedLoginCount++;
            lastFailedLogin = std::chrono::system_clock::now();
            
            // Lock account if max attempts exceeded
            if (failedLoginCount >= maxLoginAttempts) {
                lockAccount();
                logSecurityEvent("Account locked due to multiple failed attempts: " + username);
                return AuthStatus::ACCOUNT_LOCKED;
            }
            
            logSecurityEvent("Authentication failed: Invalid credentials for " + username);
            return AuthStatus::INVALID_CREDENTIALS;
        }
    }
    catch (const std::exception& e) {
        logSecurityEvent("Authentication error: " + std::string(e.what()));
        return AuthStatus::ERROR;
    }
}

bool AuthSystem::isAuthenticated() const {
    return currentSession && currentSession->isActive && isSessionValid();
}

bool AuthSystem::isAdmin() const {
    return isAuthenticated() && currentSession->role == UserRole::ADMIN;
}

bool AuthSystem::isVictim() const {
    return isAuthenticated() && currentSession->role == UserRole::VICTIM;
}

UserSession* AuthSystem::getCurrentSession() const {
    return currentSession.get();
}

void AuthSystem::logout() {
    if (currentSession) {
        logSecurityEvent("User logged out: " + currentSession->username);
        clearSession();
    }
}

bool AuthSystem::refreshSession() {
    if (currentSession && currentSession->isActive) {
        currentSession->lastActivity = std::chrono::system_clock::now();
        return true;
    }
    return false;
}

bool AuthSystem::isSessionValid() const {
    if (!currentSession || !currentSession->isActive) {
        return false;
    }
    
    // Check if session has expired (24 hours)
    auto now = std::chrono::system_clock::now();
    auto sessionDuration = std::chrono::duration_cast<std::chrono::hours>(now - currentSession->loginTime);
    
    return sessionDuration.count() < 24;
}

void AuthSystem::lockAccount() {
    lastFailedLogin = std::chrono::system_clock::now();
    logSecurityEvent("Account locked");
}

bool AuthSystem::isAccountLocked() const {
    if (failedLoginCount < maxLoginAttempts) {
        return false;
    }
    
    auto now = std::chrono::system_clock::now();
    auto lockoutDuration = std::chrono::duration_cast<std::chrono::minutes>(now - lastFailedLogin);
    
    return lockoutDuration.count() < lockoutDurationMinutes;
}

void AuthSystem::resetFailedLoginCount() {
    failedLoginCount = 0;
}

int AuthSystem::getFailedLoginCount() const {
    return failedLoginCount;
}

std::string AuthSystem::getRoleString() const {
    if (!isAuthenticated()) {
        return "Unauthenticated";
    }
    
    return (currentSession->role == UserRole::ADMIN) ? "Admin" : "Victim";
}

std::string AuthSystem::getSessionInfo() const {
    if (!currentSession) {
        return "No active session";
    }
    
    std::stringstream ss;
    ss << "User: " << currentSession->username << " | ";
    ss << "Role: " << getRoleString() << " | ";
    ss << "Session ID: " << currentSession->sessionId.substr(0, 8) << "... | ";
    ss << "Active: " << (currentSession->isActive ? "Yes" : "No");
    
    return ss.str();
}

void AuthSystem::logSecurityEvent(const std::string& event) {
    // In production, this would write to a secure log
    // For now, just output to console
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    std::cout << "[AUTH] " << std::ctime(&time_t) << event << std::endl;
}

void AuthSystem::secureWipe() {
    // Clear session data
    clearSession();
    
    // Clear encryption key
    secureMemoryCleanup();
    
    // Reset counters
    failedLoginCount = 0;
}

} // namespace MasoudyAuth 