#pragma once

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <string>

// Forward declaration
namespace MasoudyCore {
    class MasoudyCore;
}

class StealthPanel : public wxPanel {
public:
    StealthPanel(wxWindow* parent);
    virtual ~StealthPanel();

    // Core integration
    void SetCore(MasoudyCore::MasoudyCore* core);

    // UI operations
    void RefreshDisplay();
    void UpdateStatus();
    void ClearDisplay();

    // Stealth mode control
    void EnableStealthMode();
    void DisableStealthMode();
    bool IsStealthModeActive() const;

    // Exam mode control
    void EnableExamMode();
    void DisableExamMode();
    bool IsExamModeActive() const;

    // Remote control
    void EnableRemoteControl();
    void DisableRemoteControl();
    bool IsRemoteControlEnabled() const;

    // Process injection
    void InjectIntoProcess(const std::string& processName);
    void InjectScreenSharingIntoProcess(const std::string& processName);

    // Memory protection
    void EncryptMemoryRegion(void* address, size_t size);
    void ProtectMemoryRegion(void* address, size_t size);

    // Anti-detection
    void HideFromTaskManager();
    void HideFromProcessMonitor();
    void HideFromWireshark();
    void HideFromAntivirus();

    // Network obfuscation
    void EnableNetworkObfuscation();
    void DisableNetworkObfuscation();
    bool IsNetworkObfuscationEnabled() const;

    // Settings
    void LoadSettings();
    void SaveSettings();
    void ResetToDefaults();

private:
    // UI creation
    void CreateControls();

    // Event handlers
    void OnStealthModeToggle(wxCommandEvent& event);
    void OnExamModeToggle(wxCommandEvent& event);
    void OnRemoteControlToggle(wxCommandEvent& event);
    void OnProcessInjection(wxCommandEvent& event);
    void OnMemoryProtection(wxCommandEvent& event);
    void OnAntiDetection(wxCommandEvent& event);
    void OnNetworkObfuscation(wxCommandEvent& event);
    void OnRefresh(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);

    // Core reference
    MasoudyCore::MasoudyCore* m_core;

    // UI components
    wxCheckBox* m_stealthModeCheck;
    wxCheckBox* m_examModeCheck;
    wxCheckBox* m_remoteControlCheck;
    wxCheckBox* m_memoryProtectionCheck;
    wxCheckBox* m_antiDetectionCheck;
    wxCheckBox* m_networkObfuscationCheck;
    wxButton* m_refreshButton;
    wxButton* m_clearButton;
    wxStaticText* m_statusLabel;

    // State
    bool m_stealthModeActive;
    bool m_examModeActive;
    bool m_remoteControlEnabled;
    bool m_memoryProtected;
    bool m_hiddenFromDetection;
    bool m_networkObfuscationEnabled;

    DECLARE_EVENT_TABLE()
};

// Event IDs
enum {
    ID_STEALTH_MODE_TOGGLE = 2000,
    ID_EXAM_MODE_TOGGLE,
    ID_REMOTE_CONTROL_TOGGLE,
    ID_STEALTH_PROCESS_INJECTION,
    ID_INJECT_SCREEN_SHARING,
    ID_REMOVE_PROCESS,
    ID_STEALTH_MEMORY_PROTECTION,
    ID_ANTI_DETECTION,
    ID_NETWORK_OBFUSCATION,
    ID_SETTINGS_CHANGED,
    ID_REFRESH,
    ID_CLEAR,
    ID_STATUS_TIMER
}; 