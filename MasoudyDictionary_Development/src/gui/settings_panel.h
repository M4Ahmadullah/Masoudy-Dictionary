#pragma once

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>

// Forward declaration
namespace MasoudyCore {
    class MasoudyCore;
}

class SettingsPanel : public wxPanel {
public:
    SettingsPanel(wxWindow* parent);
    virtual ~SettingsPanel();

    // Settings operations
    bool LoadSettings();
    bool SaveSettings();
    bool ResetToDefaults();
    bool ExportSettings(const wxString& path);
    bool ImportSettings(const wxString& path);
    
    // UI operations
    void RefreshDisplay();
    void UpdateSettings();

    // Core integration
    void SetCore(MasoudyCore::MasoudyCore* core);

private:
    // UI components
    wxNotebook* m_notebook;
    // General settings
    wxPanel* m_generalPanel;
    wxCheckBox* m_startupCheck;
    wxCheckBox* m_minimizeToTrayCheck;
    wxCheckBox* m_autoUpdateCheck;
    wxChoice* m_languageChoice;
    wxSpinCtrl* m_autoSaveIntervalSpin;
    // Screen sharing settings
    wxPanel* m_screenSharingPanel;
    wxSpinCtrl* m_qualitySpin;
    wxSpinCtrl* m_frameRateSpin;
    wxSpinCtrl* m_compressionSpin;
    wxCheckBox* m_encryptionCheck;
    wxCheckBox* m_autoConnectCheck;
    wxSpinCtrl* m_scanTimeoutSpin;
    wxSpinCtrl* m_connectionTimeoutSpin;
    // Stealth settings
    wxPanel* m_stealthPanel;
    wxCheckBox* m_stealthModeCheck;
    wxCheckBox* m_examModeCheck;
    wxCheckBox* m_remoteControlCheck;
    wxCheckBox* m_memoryProtectionCheck;
    wxCheckBox* m_antiDetectionCheck;
    wxCheckBox* m_networkObfuscationCheck;
    wxSpinCtrl* m_stealthLevelSpin;
    // Appearance settings
    wxPanel* m_appearancePanel;
    wxChoice* m_themeChoice;
    wxCheckBox* m_fullscreenCheck;
    wxCheckBox* m_alwaysOnTopCheck;
    wxCheckBox* m_showStatusBarCheck;
    wxCheckBox* m_showToolBarCheck;
    wxSpinCtrl* m_opacitySpin;
    // Security settings
    wxPanel* m_securityPanel;
    wxCheckBox* m_encryptionEnabledCheck;
    wxCheckBox* m_compressionEnabledCheck;
    wxCheckBox* m_autoLockCheck;
    wxSpinCtrl* m_sessionTimeoutSpin;
    wxSpinCtrl* m_maxLoginAttemptsSpin;
    // Advanced settings
    wxPanel* m_advancedPanel;
    wxCheckBox* m_debugModeCheck;
    wxCheckBox* m_loggingEnabledCheck;
    wxCheckBox* m_autoBackupCheck;
    wxSpinCtrl* m_backupIntervalSpin;
    wxTextCtrl* m_logPathCtrl;
    // Core reference
    MasoudyCore::MasoudyCore* m_core;
    // UI creation
    void CreateControls();
    void CreateLayout();
    void ConnectEvents();
    void CreateGeneralPanel();
    void CreateScreenSharingPanel();
    void CreateStealthPanel();
    void CreateAppearancePanel();
    void CreateSecurityPanel();
    void CreateAdvancedPanel();
    
    // Event handlers
    void OnApply(wxCommandEvent& event);
    void OnReset(wxCommandEvent& event);
    void OnExport(wxCommandEvent& event);
    void OnImport(wxCommandEvent& event);
    void OnGenerateKey(wxCommandEvent& event);
    void OnThemeChanged(wxCommandEvent& event);
    void OnStealthLevelChanged(wxSpinEvent& event);
    
    // Helper methods
    wxString GenerateEncryptionKey();
    
    DECLARE_EVENT_TABLE()
};

// Event IDs
enum {
    ID_APPLY_BUTTON = wxID_HIGHEST + 3000,
    ID_RESET_BUTTON,
    ID_EXPORT_BUTTON,
    ID_IMPORT_BUTTON,
    ID_GENERATE_KEY_BUTTON,
    ID_THEME_CHOICE,
    ID_STEALTH_LEVEL_SPIN
}; 