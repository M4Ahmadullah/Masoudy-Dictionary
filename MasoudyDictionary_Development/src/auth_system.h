#ifndef AUTH_SYSTEM_H
#define AUTH_SYSTEM_H

#include <string>
#include <vector>
#include <memory>
#include <chrono>

namespace MasoudyAuth {

// User roles
enum class UserRole {
    VICTIM,     // Laptop 1 - Being monitored
    ADMIN       // Laptop 2 - Monitoring
};

// Authentication status
enum class AuthStatus {
    SUCCESS,
    INVALID_CREDENTIALS,
    ACCOUNT_LOCKED,
    SESSION_EXPIRED,
    ERROR
};

// User session information
struct UserSession {
    std::string username;
    UserRole role;
    std::chrono::system_clock::time_point loginTime;
    std::chrono::system_clock::time_point lastActivity;
    bool isActive;
    std::string sessionId;
};

// Authentication system class
class AuthSystem {
private:
    // Static credentials (encrypted in memory)
    static const std::string VICTIM_PASSWORD;
    static const std::string ADMIN_PASSWORD;
    
    // Session management
    std::unique_ptr<UserSession> currentSession;
    
    // Security settings
    int maxLoginAttempts;
    int lockoutDurationMinutes;
    std::chrono::system_clock::time_point lastFailedLogin;
    int failedLoginCount;
    
    // Encryption key (in memory only)
    std::vector<uint8_t> encryptionKey;
    
    // Private methods
    std::string hashPassword(const std::string& password);
    std::string generateSessionId();
    bool validateCredentials(const std::string& username, const std::string& password);
    void clearSession();
    std::string encryptString(const std::string& input);
    std::string decryptString(const std::string& input);
    void secureMemoryCleanup();

public:
    AuthSystem();
    ~AuthSystem();
    
    // Authentication methods
    AuthStatus authenticate(const std::string& username, const std::string& password);
    bool isAuthenticated() const;
    bool isAdmin() const;
    bool isVictim() const;
    
    // Session management
    UserSession* getCurrentSession() const;
    void logout();
    bool refreshSession();
    bool isSessionValid() const;
    
    // Security methods
    void lockAccount();
    bool isAccountLocked() const;
    void resetFailedLoginCount();
    int getFailedLoginCount() const;
    
    // Utility methods
    std::string getRoleString() const;
    std::string getSessionInfo() const;
    void logSecurityEvent(const std::string& event);
    
    // Memory security
    void secureWipe();
};

} // namespace MasoudyAuth

#endif // AUTH_SYSTEM_H 