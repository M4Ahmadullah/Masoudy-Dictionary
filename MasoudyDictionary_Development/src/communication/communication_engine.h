#pragma once

#include "masoudy_core.h"
#include <string>
#include <vector>
#include <functional>
#include <thread>
#include <atomic>

#ifdef _WIN32
#define MASOUDY_API __declspec(dllimport)
#else
#define MASOUDY_API
#endif

namespace MasoudyCore {

    // Communication protocols
    enum class Protocol {
        NAMED_PIPES,      // Windows named pipes
        TCP_SOCKETS,      // TCP sockets
        UDP_DATAGRAMS,    // UDP datagrams
        HTTP_PROXY,       // HTTP proxy tunneling
        HTTPS_PROXY,      // HTTPS proxy tunneling
        CUSTOM_PROTOCOL   // Custom encrypted protocol
    };

    // Message types
    enum class MessageType {
        SCREEN_DATA,      // Screen capture data
        INPUT_COMMAND,    // Input simulation command
        CONTROL_COMMAND,  // Control commands
        HEARTBEAT,       // Keep-alive message
        AUTHENTICATION,  // Authentication message
        ENCRYPTION_KEY,  // Encryption key exchange
        ERROR_MESSAGE,   // Error messages
        REMOTE_CONTROL   // Remote control commands
    };

    // Connection settings
    struct ConnectionSettings {
        Protocol protocol = Protocol::NAMED_PIPES;
        std::string targetIP = "";
        int targetPort = 0;
        std::string pipeName = "MasoudyPipe";
        bool enableEncryption = true;
        bool enableCompression = true;
        int timeoutMs = 5000;
        int retryAttempts = 3;
        bool enableHeartbeat = true;
        int heartbeatIntervalMs = 30000;
    };

    // Message structure
    struct Message {
        MessageType type;
        uint32_t sequenceNumber;
        uint32_t dataSize;
        std::vector<uint8_t> data;
        uint64_t timestamp;
    };

    // Communication engine class
    class MASOUDY_API CommunicationEngine {
    public:
        CommunicationEngine();
        ~CommunicationEngine();

        // Initialization
        bool Initialize(const ConnectionSettings& settings);
        void Shutdown();

        // Connection management
        bool Connect(const std::string& target);
        bool Disconnect();
        bool IsConnected() const;
        ConnectionStatus GetStatus() const;

        // Message sending
        bool SendMessage(const Message& message);
        bool SendScreenData(const std::vector<uint8_t>& screenData);
        bool SendInputCommand(const std::string& command);
        bool SendControlCommand(const std::string& command);
        bool SendHeartbeat();
        
        // Remote control
        bool SendRemoteControlCommand(const std::string& command);
        bool ProcessRemoteControlMessage(const Message& message);
        void SetInputHandler(InputHandler* handler);

        // Message receiving
        bool ReceiveMessage(Message& message);
        bool HasMessage() const;
        std::vector<Message> GetAllMessages();

        // Security
        bool EnableEncryption();
        bool DisableEncryption();
        bool IsEncryptionEnabled() const;
        bool ExchangeEncryptionKeys();
        bool Authenticate(const std::string& credentials);

        // Performance
        void SetCompressionLevel(int level);
        void SetEncryptionLevel(int level);
        int GetCompressionLevel() const;
        int GetEncryptionLevel() const;

        // Statistics
        uint64_t GetBytesSent() const;
        uint64_t GetBytesReceived() const;
        uint32_t GetMessagesSent() const;
        uint32_t GetMessagesReceived() const;
        double GetAverageLatency() const;

        // Event callbacks
        using ConnectionCallback = std::function<void(ConnectionStatus)>;
        using MessageCallback = std::function<void(const Message&)>;
        using ErrorCallback = std::function<void(const std::string&)>;

        void SetConnectionCallback(ConnectionCallback callback);
        void SetMessageCallback(MessageCallback callback);
        void SetErrorCallback(ErrorCallback callback);

        // Settings management
        void SetSettings(const ConnectionSettings& settings);
        ConnectionSettings GetSettings() const;
        
        // Remote control
        bool EnableRemoteControl(bool enable);
        bool SetRemoteOnlyMode(bool enable);
        bool SetEncryption(bool enable);

    private:
        // Settings
        ConnectionSettings m_settings;
        ConnectionStatus m_status;
        bool m_encryptionEnabled;
        bool m_initialized;

        // Network socket
        int m_socket;

        // Performance settings
        int m_compressionLevel;
        int m_encryptionLevel;

        // Statistics
        uint64_t m_bytesSent;
        uint64_t m_bytesReceived;
        uint32_t m_messagesSent;
        uint32_t m_messagesReceived;
        double m_averageLatency;

        // Callbacks
        ConnectionCallback m_connectionCallback;
        MessageCallback m_messageCallback;
        ErrorCallback m_errorCallback;

        // Threading
        std::thread m_receiveThread;
        std::thread m_heartbeatThread;
        std::atomic<bool> m_running;
        
        // Remote control
        bool m_remoteControlEnabled;
        bool m_remoteOnlyMode;
        InputHandler* m_inputHandler;

        // Internal methods
        bool InitializeNamedPipes();
        bool InitializeTCPSockets();
        bool InitializeUDPDatagrams();
        bool InitializeHTTPProxy();
        bool InitializeHTTPSProxy();
        bool InitializeCustomProtocol();

        // Message handling
        bool SendMessageInternal(const Message& message);
        bool ReceiveMessageInternal(Message& message);
        void ProcessIncomingMessage(const Message& message);
        void HandleConnectionStatusChange(ConnectionStatus status);

        // Security methods
        bool EncryptMessage(Message& message);
        bool DecryptMessage(Message& message);
        bool CompressMessage(Message& message);
        bool DecompressMessage(Message& message);
        std::vector<uint8_t> GenerateEncryptionKey();
        bool ValidateMessage(const Message& message);

        // Threading methods
        void ReceiveThreadProc();
        void HeartbeatThreadProc();
        void StartReceiveThread();
        void StopReceiveThread();

        // Utility methods
        uint32_t GenerateSequenceNumber();
        uint64_t GetCurrentTimestamp();
        std::string GetErrorMessage(int errorCode);
        void UpdateStatistics(uint64_t bytesSent, uint64_t bytesReceived);
    };

    // Utility functions
    MASOUDY_API std::string GetProtocolName(Protocol protocol);
    MASOUDY_API std::string GetMessageTypeName(MessageType type);
    MASOUDY_API std::string GetConnectionStatusName(ConnectionStatus status);
    MASOUDY_API bool IsValidIPAddress(const std::string& ip);
    MASOUDY_API bool IsValidPort(int port);
}  