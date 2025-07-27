#pragma once

#include <string>
#include <vector>

namespace MasoudyCore {

// Forward declarations
enum class InjectionMethod {
    DLL_INJECTION,
    CODE_INJECTION,
    PROCESS_HOLLOWING,
    THREAD_HIJACKING,
    APC_INJECTION,
    REFLECTIVE_INJECTION
};

struct ProcessInfo {
    uint32_t processId;
    std::string processName;
    std::string executablePath;
    bool is64Bit;
    bool isElevated;
};

struct InjectionSettings {
    InjectionMethod method;
    bool stealthMode;
    bool elevatePrivileges;
    bool hideInjection;
    
    InjectionSettings() : method(InjectionMethod::DLL_INJECTION), 
                         stealthMode(true), elevatePrivileges(false), 
                         hideInjection(true) {}
};

class ProcessInjector {
public:
    ProcessInjector();
    ~ProcessInjector();
    
    bool Initialize();
    void Shutdown();
    bool InjectIntoProcess(const std::string& processName, const std::string& dllPath = "");
    bool InjectIntoProcessById(uint32_t processId, const std::string& dllPath);
    bool InjectCodeIntoProcess(const std::string& processName, const std::vector<uint8_t>& code);
    bool InjectCodeIntoProcessById(uint32_t processId, const std::vector<uint8_t>& code);
    bool InjectScreenSharing(const std::string& processName);
    bool UninjectFromProcess(const std::string& processName);
    bool RemoveInjection();
    bool TerminateInjection();
    
    // Process information
    std::vector<ProcessInfo> GetRunningProcesses();
    ProcessInfo GetProcessInfo(const std::string& processName);
    ProcessInfo GetProcessInfoById(uint32_t processId);
    bool IsProcessRunning(const std::string& processName);
    bool IsProcessRunningById(uint32_t processId);
    
    // Settings
    bool SetInjectionSettings(const InjectionSettings& settings);
    InjectionSettings GetInjectionSettings() const;
    void SetInjectionMethod(InjectionMethod method);
    InjectionMethod GetInjectionMethod() const;
    
    // Status
    bool IsInitialized() const;
    bool IsInjectionSuccessful() const;
    std::string GetLastError() const;

private:
    bool m_initialized;
    bool m_injectionSuccessful;
    std::string m_lastError;
    InjectionSettings m_settings;
    
    // Helper methods
    uint32_t GetProcessIdByName(const std::string& processName);
    std::string GetProcessNameById(uint32_t processId);
    bool ValidateProcess(const std::string& processName);
    bool ValidateProcess(uint32_t processId);
    bool ValidateDllPath(const std::string& dllPath);
    bool ValidateDLL(const std::string& dllPath);
    
    // Internal methods
    bool InitializeInjectionSystem();
    void ShutdownInjectionSystem();
    bool PerformDLLInjection(uint32_t processId, const std::string& dllPath);
    bool PerformCodeInjection(uint32_t processId, const std::vector<uint8_t>& code);
    bool PerformProcessHollowing(const std::string& processName, const std::string& dllPath);
    bool PerformThreadHijacking(uint32_t processId, const std::string& dllPath);
    bool PerformAPCInjection(uint32_t processId, const std::string& dllPath);
    bool PerformReflectiveInjection(uint32_t processId, const std::string& dllPath);
    bool ElevatePrivileges();
    bool HideInjection();
};

// Utility functions
std::string GetInjectionMethodName(InjectionMethod method);
bool IsInjectionMethodSupported(InjectionMethod method);
std::vector<std::string> GetInjectableProcesses();
bool IsProcessInjectable(const std::string& processName);
bool IsProcessInjectableById(uint32_t processId);
uint32_t GetProcessIdByName(const std::string& processName);
std::string GetProcessNameById(uint32_t processId);

} // namespace MasoudyCore 