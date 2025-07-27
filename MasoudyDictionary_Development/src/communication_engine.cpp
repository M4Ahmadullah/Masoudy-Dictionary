#include "communication_engine.h"
#include "input_handler.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <algorithm>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#endif

namespace MasoudyCore {

CommunicationEngine::CommunicationEngine() 
    : m_status(ConnectionStatus::DISCONNECTED)
    , m_encryptionEnabled(false)
    , m_initialized(false)
    , m_bytesSent(0)
    , m_bytesReceived(0)
    , m_messagesSent(0)
    , m_messagesReceived(0)
    , m_averageLatency(0.0)
    , m_running(false)
    , m_socket(-1) {
    
#ifdef _WIN32
    // Initialize Winsock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

CommunicationEngine::~CommunicationEngine() {
    Shutdown();
#ifdef _WIN32
    WSACleanup();
#endif
}

bool CommunicationEngine::Initialize(const ConnectionSettings& settings) {
    m_settings = settings;
    m_initialized = true;
    return true;
}

void CommunicationEngine::Shutdown() {
    if (m_initialized) {
        Disconnect();
        m_initialized = false;
    }
}

bool CommunicationEngine::Connect(const std::string& target) {
    if (!m_initialized) {
        return false;
    }
    
    m_status = ConnectionStatus::CONNECTING;
    
    try {
        // Parse target (IP:port or hostname:port)
        std::string host;
        int port = m_settings.targetPort;
        
        size_t colonPos = target.find(':');
        if (colonPos != std::string::npos) {
            host = target.substr(0, colonPos);
            port = std::stoi(target.substr(colonPos + 1));
        } else {
            host = target;
        }
        
        // Create socket
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_socket == -1) {
            m_status = ConnectionStatus::ERROR;
            return false;
        }
        
        // Set socket options
        int opt = 1;
        setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
        
        // Connect to target
        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        
        if (inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr) <= 0) {
            // Try to resolve hostname
            struct hostent* he = gethostbyname(host.c_str());
            if (he == nullptr) {
                m_status = ConnectionStatus::ERROR;
                return false;
            }
            serverAddr.sin_addr = *((struct in_addr*)he->h_addr);
        }
        
        if (connect(m_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            m_status = ConnectionStatus::ERROR;
            return false;
        }
        
        // Start receive thread
        StartReceiveThread();
        
        m_status = ConnectionStatus::CONNECTED;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Connection failed: " << e.what() << std::endl;
        m_status = ConnectionStatus::ERROR;
        return false;
    }
}

bool CommunicationEngine::Disconnect() {
    if (m_status == ConnectionStatus::DISCONNECTED) {
        return true;
    }
    
    try {
        // Stop receive thread
        StopReceiveThread();
        
        // Close socket
        if (m_socket != -1) {
#ifdef _WIN32
            closesocket(m_socket);
#else
            close(m_socket);
#endif
            m_socket = -1;
        }
        
        m_status = ConnectionStatus::DISCONNECTED;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Disconnect failed: " << e.what() << std::endl;
        return false;
    }
}

bool CommunicationEngine::IsConnected() const {
    return m_status == ConnectionStatus::CONNECTED;
}

ConnectionStatus CommunicationEngine::GetStatus() const {
    return m_status;
}

bool CommunicationEngine::SendMessage(const Message& message) {
    if (!IsConnected()) {
        return false;
    }
    
    try {
        // Serialize message
        std::vector<uint8_t> data;
        
        // Message header
        uint32_t type = static_cast<uint32_t>(message.type);
        uint32_t seq = message.sequenceNumber;
        uint32_t size = message.dataSize;
        uint64_t timestamp = message.timestamp;
        
        // Add header
        data.insert(data.end(), (uint8_t*)&type, (uint8_t*)&type + sizeof(type));
        data.insert(data.end(), (uint8_t*)&seq, (uint8_t*)&seq + sizeof(seq));
        data.insert(data.end(), (uint8_t*)&size, (uint8_t*)&size + sizeof(size));
        data.insert(data.end(), (uint8_t*)&timestamp, (uint8_t*)&timestamp + sizeof(timestamp));
        
        // Add message data
        data.insert(data.end(), message.data.begin(), message.data.end());
        
        // Encrypt if enabled
        if (m_encryptionEnabled) {
            EncryptMessage(const_cast<Message&>(message));
        }
        
        // Send data
        int bytesSent = send(m_socket, (char*)data.data(), data.size(), 0);
        if (bytesSent > 0) {
            m_bytesSent += bytesSent;
            m_messagesSent++;
            return true;
        }
        
        return false;
    }
    catch (const std::exception& e) {
        std::cerr << "Send message failed: " << e.what() << std::endl;
        return false;
    }
}

bool CommunicationEngine::SendScreenData(const std::vector<uint8_t>& screenData) {
    Message message;
    message.type = MessageType::SCREEN_DATA;
    message.data = screenData;
    message.dataSize = static_cast<uint32_t>(screenData.size());
    message.sequenceNumber = GenerateSequenceNumber();
    message.timestamp = GetCurrentTimestamp();
    
    return SendMessage(message);
}

bool CommunicationEngine::SendInputCommand(const std::string& command) {
    Message message;
    message.type = MessageType::INPUT_COMMAND;
    message.data = std::vector<uint8_t>(command.begin(), command.end());
    message.dataSize = static_cast<uint32_t>(command.size());
    message.sequenceNumber = GenerateSequenceNumber();
    message.timestamp = GetCurrentTimestamp();
    
    return SendMessage(message);
}

bool CommunicationEngine::SendControlCommand(const std::string& command) {
    Message message;
    message.type = MessageType::CONTROL_COMMAND;
    message.data = std::vector<uint8_t>(command.begin(), command.end());
    message.dataSize = static_cast<uint32_t>(command.size());
    message.sequenceNumber = GenerateSequenceNumber();
    message.timestamp = GetCurrentTimestamp();
    
    return SendMessage(message);
}

bool CommunicationEngine::SendHeartbeat() {
    Message message;
    message.type = MessageType::HEARTBEAT;
    message.sequenceNumber = GenerateSequenceNumber();
    message.timestamp = GetCurrentTimestamp();
    
    return SendMessage(message);
}

bool CommunicationEngine::ReceiveMessage(Message& message) {
    if (!IsConnected()) {
        return false;
    }
    
    try {
        // Receive header
        uint8_t header[20];
        int bytesReceived = recv(m_socket, (char*)header, sizeof(header), 0);
        
        if (bytesReceived <= 0) {
            return false;
        }
        
        // Parse header
        uint32_t type, seq, size;
        uint64_t timestamp;
        
        memcpy(&type, header, sizeof(type));
        memcpy(&seq, header + 4, sizeof(seq));
        memcpy(&size, header + 8, sizeof(size));
        memcpy(&timestamp, header + 12, sizeof(timestamp));
        
        message.type = static_cast<MessageType>(type);
        message.sequenceNumber = seq;
        message.dataSize = size;
        message.timestamp = timestamp;
        
        // Receive data
        if (size > 0) {
            message.data.resize(size);
            bytesReceived = recv(m_socket, (char*)message.data.data(), size, 0);
            
            if (bytesReceived > 0) {
                m_bytesReceived += bytesReceived;
                m_messagesReceived++;
                
                // Decrypt if enabled
                if (m_encryptionEnabled) {
                    DecryptMessage(message);
                }
                
                return true;
            }
        }
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Receive message failed: " << e.what() << std::endl;
        return false;
    }
}

bool CommunicationEngine::HasMessage() const {
    // Check if there's data available on socket
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(m_socket, &readfds);
    
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000; // 1ms timeout
    
    int result = select(m_socket + 1, &readfds, nullptr, nullptr, &timeout);
    return result > 0 && FD_ISSET(m_socket, &readfds);
}

std::vector<Message> CommunicationEngine::GetAllMessages() {
    std::vector<Message> messages;
    
    while (HasMessage()) {
        Message message;
        if (ReceiveMessage(message)) {
            messages.push_back(message);
        }
    }
    
    return messages;
}

bool CommunicationEngine::EnableEncryption() {
    m_encryptionEnabled = true;
    return true;
}

bool CommunicationEngine::DisableEncryption() {
    m_encryptionEnabled = false;
    return true;
}

bool CommunicationEngine::IsEncryptionEnabled() const {
    return m_encryptionEnabled;
}

bool CommunicationEngine::ExchangeEncryptionKeys() {
    try {
        // Generate encryption key
        std::vector<uint8_t> key = GenerateEncryptionKey();
        
        // Send key to remote
        Message keyMessage;
        keyMessage.type = MessageType::ENCRYPTION_KEY;
        keyMessage.data = key;
        keyMessage.dataSize = static_cast<uint32_t>(key.size());
        keyMessage.sequenceNumber = GenerateSequenceNumber();
        keyMessage.timestamp = GetCurrentTimestamp();
        
        return SendMessage(keyMessage);
    }
    catch (const std::exception& e) {
        std::cerr << "Key exchange failed: " << e.what() << std::endl;
        return false;
    }
}

bool CommunicationEngine::Authenticate(const std::string& credentials) {
    try {
        Message authMessage;
        authMessage.type = MessageType::AUTHENTICATION;
        authMessage.data = std::vector<uint8_t>(credentials.begin(), credentials.end());
        authMessage.dataSize = static_cast<uint32_t>(credentials.size());
        authMessage.sequenceNumber = GenerateSequenceNumber();
        authMessage.timestamp = GetCurrentTimestamp();
        
        return SendMessage(authMessage);
    }
    catch (const std::exception& e) {
        std::cerr << "Authentication failed: " << e.what() << std::endl;
        return false;
    }
}

void CommunicationEngine::SetCompressionLevel(int level) {
    m_compressionLevel = std::max(0, std::min(100, level));
}

void CommunicationEngine::SetEncryptionLevel(int level) {
    m_encryptionLevel = std::max(0, std::min(100, level));
}

int CommunicationEngine::GetCompressionLevel() const {
    return m_compressionLevel;
}

int CommunicationEngine::GetEncryptionLevel() const {
    return m_encryptionLevel;
}

uint64_t CommunicationEngine::GetBytesSent() const {
    return m_bytesSent;
}

uint64_t CommunicationEngine::GetBytesReceived() const {
    return m_bytesReceived;
}

uint32_t CommunicationEngine::GetMessagesSent() const {
    return m_messagesSent;
}

uint32_t CommunicationEngine::GetMessagesReceived() const {
    return m_messagesReceived;
}

double CommunicationEngine::GetAverageLatency() const {
    return m_averageLatency;
}

void CommunicationEngine::SetConnectionCallback(ConnectionCallback callback) {
    m_connectionCallback = callback;
}

void CommunicationEngine::SetMessageCallback(MessageCallback callback) {
    m_messageCallback = callback;
}

void CommunicationEngine::SetErrorCallback(ErrorCallback callback) {
    m_errorCallback = callback;
}

void CommunicationEngine::SetSettings(const ConnectionSettings& settings) {
    m_settings = settings;
}

ConnectionSettings CommunicationEngine::GetSettings() const {
    return m_settings;
}

bool CommunicationEngine::EnableRemoteControl(bool enable) {
    m_remoteControlEnabled = enable;
    return true;
}

bool CommunicationEngine::SetRemoteOnlyMode(bool enable) {
    m_remoteOnlyMode = enable;
    return true;
}

bool CommunicationEngine::SetEncryption(bool enable) {
    if (enable) {
        return EnableEncryption();
    } else {
        return DisableEncryption();
    }
}

// Internal methods implementation
bool CommunicationEngine::InitializeNamedPipes() { 
    // Windows named pipes implementation would go here
    return true; 
}

bool CommunicationEngine::InitializeTCPSockets() { 
    // TCP sockets are already implemented in Connect()
    return true; 
}

bool CommunicationEngine::InitializeUDPDatagrams() { 
    // UDP implementation would go here
    return true; 
}

bool CommunicationEngine::InitializeHTTPProxy() { 
    // HTTP proxy implementation would go here
    return true; 
}

bool CommunicationEngine::InitializeHTTPSProxy() { 
    // HTTPS proxy implementation would go here
    return true; 
}

bool CommunicationEngine::InitializeCustomProtocol() { 
    // Custom protocol implementation would go here
    return true; 
}

bool CommunicationEngine::SendMessageInternal(const Message& message) { 
    return SendMessage(message); 
}

bool CommunicationEngine::ReceiveMessageInternal(Message& message) { 
    return ReceiveMessage(message); 
}

void CommunicationEngine::ProcessIncomingMessage(const Message& message) {
    if (m_messageCallback) {
        m_messageCallback(message);
    }
}

void CommunicationEngine::HandleConnectionStatusChange(ConnectionStatus status) {
    if (m_connectionCallback) {
        m_connectionCallback(status);
    }
}

bool CommunicationEngine::EncryptMessage(Message& message) {
    // Simple XOR encryption for demonstration
    for (size_t i = 0; i < message.data.size(); ++i) {
        message.data[i] ^= 0x42; // Simple XOR key
    }
    return true;
}

bool CommunicationEngine::DecryptMessage(Message& message) {
    // XOR decryption (symmetric)
    return EncryptMessage(message);
}

bool CommunicationEngine::CompressMessage(Message& message) {
    // Simple compression - in real implementation, use zlib or similar
    if (message.data.size() > 100) {
        // For demonstration, just reduce size by half
        std::vector<uint8_t> compressed;
        for (size_t i = 0; i < message.data.size(); i += 2) {
            compressed.push_back(message.data[i]);
        }
        message.data = compressed;
        message.dataSize = static_cast<uint32_t>(compressed.size());
    }
    return true;
}

bool CommunicationEngine::DecompressMessage(Message& message) {
    // Simple decompression
    if (message.dataSize < message.data.size() * 2) {
        std::vector<uint8_t> decompressed;
        for (uint8_t byte : message.data) {
            decompressed.push_back(byte);
            decompressed.push_back(byte); // Duplicate for demonstration
        }
        message.data = decompressed;
        message.dataSize = static_cast<uint32_t>(decompressed.size());
    }
    return true;
}

std::vector<uint8_t> CommunicationEngine::GenerateEncryptionKey() {
    std::vector<uint8_t> key(32);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    for (auto& byte : key) {
        byte = static_cast<uint8_t>(dis(gen));
    }
    
    return key;
}

bool CommunicationEngine::ValidateMessage(const Message& message) {
    return message.dataSize == message.data.size() && 
           message.sequenceNumber > 0 && 
           message.timestamp > 0;
}

void CommunicationEngine::ReceiveThreadProc() {
    while (m_running) {
        if (HasMessage()) {
            Message message;
            if (ReceiveMessage(message)) {
                ProcessIncomingMessage(message);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void CommunicationEngine::HeartbeatThreadProc() {
    while (m_running) {
        if (IsConnected()) {
            SendHeartbeat();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(m_settings.heartbeatIntervalMs));
    }
}

void CommunicationEngine::StartReceiveThread() {
    m_running = true;
    m_receiveThread = std::thread(&CommunicationEngine::ReceiveThreadProc, this);
    
    if (m_settings.enableHeartbeat) {
        m_heartbeatThread = std::thread(&CommunicationEngine::HeartbeatThreadProc, this);
    }
}

void CommunicationEngine::StopReceiveThread() {
    m_running = false;
    
    if (m_receiveThread.joinable()) {
        m_receiveThread.join();
    }
    
    if (m_heartbeatThread.joinable()) {
        m_heartbeatThread.join();
    }
}

uint32_t CommunicationEngine::GenerateSequenceNumber() {
    static std::atomic<uint32_t> sequence(0);
    return ++sequence;
}

uint64_t CommunicationEngine::GetCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()).count();
}

std::string CommunicationEngine::GetErrorMessage(int errorCode) {
    switch (errorCode) {
        case 0: return "Success";
        case 1: return "Connection failed";
        case 2: return "Send failed";
        case 3: return "Receive failed";
        case 4: return "Invalid message";
        default: return "Unknown error";
    }
}

void CommunicationEngine::UpdateStatistics(uint64_t bytesSent, uint64_t bytesReceived) {
    m_bytesSent += bytesSent;
    m_bytesReceived += bytesReceived;
}

// Utility functions
std::string GetProtocolName(Protocol protocol) {
    switch (protocol) {
        case Protocol::NAMED_PIPES: return "Named Pipes";
        case Protocol::TCP_SOCKETS: return "TCP Sockets";
        case Protocol::UDP_DATAGRAMS: return "UDP Datagrams";
        case Protocol::HTTP_PROXY: return "HTTP Proxy";
        case Protocol::HTTPS_PROXY: return "HTTPS Proxy";
        case Protocol::CUSTOM_PROTOCOL: return "Custom Protocol";
        default: return "Unknown";
    }
}

std::string GetMessageTypeName(MessageType type) {
    switch (type) {
        case MessageType::SCREEN_DATA: return "Screen Data";
        case MessageType::INPUT_COMMAND: return "Input Command";
        case MessageType::CONTROL_COMMAND: return "Control Command";
        case MessageType::HEARTBEAT: return "Heartbeat";
        case MessageType::AUTHENTICATION: return "Authentication";
        case MessageType::ENCRYPTION_KEY: return "Encryption Key";
        case MessageType::ERROR_MESSAGE: return "Error Message";
        default: return "Unknown";
    }
}

std::string GetConnectionStatusName(ConnectionStatus status) {
    switch (status) {
        case ConnectionStatus::DISCONNECTED: return "Disconnected";
        case ConnectionStatus::CONNECTING: return "Connecting";
        case ConnectionStatus::CONNECTED: return "Connected";
        case ConnectionStatus::ERROR: return "Error";
        default: return "Unknown";
    }
}

bool IsValidIPAddress(const std::string& ip) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr)) == 1;
}

bool IsValidPort(int port) {
    return port >= 1 && port <= 65535;
}

bool CommunicationEngine::SendRemoteControlCommand(const std::string& command) {
    Message message;
    message.type = MessageType::REMOTE_CONTROL;
    message.data = std::vector<uint8_t>(command.begin(), command.end());
    message.dataSize = static_cast<uint32_t>(command.size());
    message.sequenceNumber = GenerateSequenceNumber();
    message.timestamp = GetCurrentTimestamp();
    
    return SendMessage(message);
}

bool CommunicationEngine::ProcessRemoteControlMessage(const Message& message) {
    if (message.type != MessageType::REMOTE_CONTROL) {
        return false;
    }
    
    try {
        // Convert message data back to string
        std::string command(message.data.begin(), message.data.end());
        
        // Process the remote control command
        if (m_inputHandler) {
            return m_inputHandler->InjectRemoteCommand(command);
        }
        
        return false;
    }
    catch (const std::exception& e) {
        std::cerr << "Remote control processing failed: " << e.what() << std::endl;
        return false;
    }
}

void CommunicationEngine::SetInputHandler(InputHandler* handler) {
    m_inputHandler = handler;
}

} // namespace MasoudyCore 