#ifndef MASOUDY_CORE_H
#define MASOUDY_CORE_H

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <thread>
#include "memory_protection.h"  // For ProtectionType enum

namespace MasoudyCore {

// Forward declarations
class StealthEngine;
class CommunicationEngine;
class ScreenCapture;
class InputHandler;
class NetworkScanner;
class MemoryProtection;
class ProcessInjector;

enum class AppMode {
    STEALTH_MODE,
    SCREEN_SHARING,
    RESEARCH_MODE
};

enum class ConnectionStatus {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    ERROR
};

struct ScreenSharingSettings {
    std::string target;
    int quality;
    int frameRate;
    int compression;
    bool encryption;
    bool remoteOnly;
    bool examMode;
};

struct NetworkHost {
    std::string ip;
    std::string hostname;
    std::string os;
    std::vector<std::string> services;
    bool accessible;
};

class MasoudyCore {
public:
    MasoudyCore();
    ~MasoudyCore();

    // Initialization and shutdown
    bool Initialize(AppMode mode = AppMode::STEALTH_MODE);
    void Shutdown();

    // Application mode management
    bool SetApplicationMode(AppMode mode);
    AppMode GetCurrentMode() const;

    // Screen sharing functionality
    bool StartScreenSharing(const ScreenSharingSettings& settings);
    bool StopScreenSharing();
    bool IsScreenSharingActive() const;
    ConnectionStatus GetConnectionStatus() const;

    // Network scanning functionality
    bool ScanNetwork(std::vector<NetworkHost>& hosts);
    bool ConnectToTarget(const std::string& target);

    // Stealth functionality
    bool EnableStealthMode();
    bool DisableStealthMode();
    bool IsStealthModeActive() const;

    // Exam mode functionality - complete app closure with background operation
    bool EnableExamMode();
    bool DisableExamMode();
    bool IsExamModeActive() const;

    // Process injection functionality
    bool InjectIntoProcess(const std::string& processName);
    bool InjectScreenSharingIntoProcess(const std::string& processName);
    bool HideFromTaskManager();
    bool HideFromProcessMonitor();
    bool HideFromWireshark();
    bool HideFromAntivirus();

    // Memory protection functionality
    bool EncryptMemoryRegion(void* address, size_t size);
    bool ProtectMemoryRegion(void* address, size_t size);

    // Remote control functionality
    bool EnableRemoteControl();
    bool DisableRemoteControl();

    // Application control functionality
    void CloseApplication();
    bool IsApplicationClosed() const;

    // Callback setters
    void SetConnectionCallback(std::function<void(ConnectionStatus)> callback);
    void SetScreenDataCallback(std::function<void(const std::vector<uint8_t>&)> callback);
    void SetErrorCallback(std::function<void(const std::string&)> callback);

    // Utility functions
    std::string GetVersion() const;
    std::string GetBuildInfo() const;
    bool IsRunningAsAdmin() const;
    std::string GetSystemInfo() const;

    // Settings management
    bool LoadSettings(const std::string& path);
    bool SaveSettings(const std::string& path);

private:
    // Application state
    AppMode m_currentMode;
    bool m_initialized;
    bool m_screenSharingActive;
    bool m_stealthModeActive;
    bool m_examModeActive;
    bool m_appClosed;
    std::string m_currentTarget;

    // Core components
    std::unique_ptr<StealthEngine> m_stealthEngine;
    std::unique_ptr<CommunicationEngine> m_communicationEngine;
    std::unique_ptr<ScreenCapture> m_screenCapture;
    std::unique_ptr<InputHandler> m_inputHandler;
    std::unique_ptr<NetworkScanner> m_networkScanner;
    std::unique_ptr<MemoryProtection> m_memoryProtection;
    std::unique_ptr<ProcessInjector> m_processInjector;

    // Threading
    std::thread m_captureThread;
    std::thread m_examThread;

    // Callbacks
    std::function<void(ConnectionStatus)> m_connectionCallback;
    std::function<void(const std::vector<uint8_t>&)> m_screenDataCallback;
    std::function<void(const std::string&)> m_errorCallback;

    // Internal methods
    void NotifyConnectionStatus(ConnectionStatus status);
    void NotifyScreenData(const std::vector<uint8_t>& data);
    void NotifyError(const std::string& error);
};

} // namespace MasoudyCore

#endif // MASOUDY_CORE_H 