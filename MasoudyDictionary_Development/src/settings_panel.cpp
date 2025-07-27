#include "settings_panel.h"
#include "masoudy_core.h"
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
#include <random>
#include <iostream>
#include <sstream>
#include <iomanip>

BEGIN_EVENT_TABLE(SettingsPanel, wxPanel)
    EVT_BUTTON(ID_APPLY_BUTTON, SettingsPanel::OnApply)
    EVT_BUTTON(ID_RESET_BUTTON, SettingsPanel::OnReset)
    EVT_BUTTON(ID_EXPORT_BUTTON, SettingsPanel::OnExport)
    EVT_BUTTON(ID_IMPORT_BUTTON, SettingsPanel::OnImport)
    EVT_BUTTON(ID_GENERATE_KEY_BUTTON, SettingsPanel::OnGenerateKey)
    EVT_CHOICE(ID_THEME_CHOICE, SettingsPanel::OnThemeChanged)
    EVT_SPINCTRL(ID_STEALTH_LEVEL_SPIN, SettingsPanel::OnStealthLevelChanged)
END_EVENT_TABLE()

SettingsPanel::SettingsPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY)
    , m_core(nullptr) {
    
    std::cout << "SettingsPanel constructor called" << std::endl;
    
    CreateControls();
    CreateLayout();
    ConnectEvents();
}

SettingsPanel::~SettingsPanel() {
    std::cout << "SettingsPanel destructor called" << std::endl;
}

void SettingsPanel::SetCore(MasoudyCore::MasoudyCore* core) {
    m_core = core;
}

bool SettingsPanel::LoadSettings() {
    std::cout << "Loading settings" << std::endl;
    return true;
}

bool SettingsPanel::SaveSettings() {
    std::cout << "Saving settings" << std::endl;
    return true;
}

bool SettingsPanel::ResetToDefaults() {
    std::cout << "Resetting settings to defaults" << std::endl;
    return true;
}

bool SettingsPanel::ExportSettings(const wxString& path) {
    std::cout << "Exporting settings to: " << path << std::endl;
    return true;
}

bool SettingsPanel::ImportSettings(const wxString& path) {
    std::cout << "Importing settings from: " << path << std::endl;
    return true;
}

void SettingsPanel::RefreshDisplay() {
    std::cout << "Refreshing settings display" << std::endl;
}

void SettingsPanel::UpdateSettings() {
    std::cout << "Updating settings" << std::endl;
}

void SettingsPanel::CreateControls() {
    // Create notebook
    m_notebook = new wxNotebook(this, wxID_ANY);

    // Create all category panels
    CreateGeneralPanel();
    CreateScreenSharingPanel();
    CreateStealthPanel();
    CreateAppearancePanel();
    CreateSecurityPanel();
    CreateAdvancedPanel();
}

void SettingsPanel::CreateLayout() {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(m_notebook, 1, wxEXPAND | wxALL, 10);

    // Button row
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* applyBtn = new wxButton(this, ID_APPLY_BUTTON, wxT("Apply"));
    wxButton* resetBtn = new wxButton(this, ID_RESET_BUTTON, wxT("Reset"));
    wxButton* exportBtn = new wxButton(this, ID_EXPORT_BUTTON, wxT("Export"));
    wxButton* importBtn = new wxButton(this, ID_IMPORT_BUTTON, wxT("Import"));
    wxButton* genKeyBtn = new wxButton(this, ID_GENERATE_KEY_BUTTON, wxT("Generate Key"));
    buttonSizer->Add(applyBtn, 0, wxALL, 5);
    buttonSizer->Add(resetBtn, 0, wxALL, 5);
    buttonSizer->Add(exportBtn, 0, wxALL, 5);
    buttonSizer->Add(importBtn, 0, wxALL, 5);
    buttonSizer->Add(genKeyBtn, 0, wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxBOTTOM | wxRIGHT, 10);
    SetSizer(mainSizer);
}

void SettingsPanel::ConnectEvents() {
    // All events are already connected via the event table
}

void SettingsPanel::CreateGeneralPanel() {
    m_generalPanel = new wxPanel(m_notebook);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    m_startupCheck = new wxCheckBox(m_generalPanel, wxID_ANY, wxT("Start with Windows"));
    m_minimizeToTrayCheck = new wxCheckBox(m_generalPanel, wxID_ANY, wxT("Minimize to tray"));
    m_autoUpdateCheck = new wxCheckBox(m_generalPanel, wxID_ANY, wxT("Enable auto-update"));
    m_languageChoice = new wxChoice(m_generalPanel, wxID_ANY);
    m_languageChoice->Append(wxT("English"));
    m_languageChoice->Append(wxT("فارسی"));
    m_languageChoice->SetSelection(0);
    m_autoSaveIntervalSpin = new wxSpinCtrl(m_generalPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 60, 5);
    sizer->Add(m_startupCheck, 0, wxALL, 5);
    sizer->Add(m_minimizeToTrayCheck, 0, wxALL, 5);
    sizer->Add(m_autoUpdateCheck, 0, wxALL, 5);
    sizer->Add(new wxStaticText(m_generalPanel, wxID_ANY, wxT("Language:")), 0, wxALL, 5);
    sizer->Add(m_languageChoice, 0, wxALL, 5);
    sizer->Add(new wxStaticText(m_generalPanel, wxID_ANY, wxT("Auto-save interval (min):")), 0, wxALL, 5);
    sizer->Add(m_autoSaveIntervalSpin, 0, wxALL, 5);
    m_generalPanel->SetSizer(sizer);
    m_notebook->AddPage(m_generalPanel, wxT("General"));
}

void SettingsPanel::CreateScreenSharingPanel() {
    m_screenSharingPanel = new wxPanel(m_notebook);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    m_qualitySpin = new wxSpinCtrl(m_screenSharingPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, 80);
    m_frameRateSpin = new wxSpinCtrl(m_screenSharingPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 60, 30);
    m_compressionSpin = new wxSpinCtrl(m_screenSharingPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, 70);
    m_encryptionCheck = new wxCheckBox(m_screenSharingPanel, wxID_ANY, wxT("Enable Encryption"));
    m_autoConnectCheck = new wxCheckBox(m_screenSharingPanel, wxID_ANY, wxT("Auto-connect on startup"));
    m_scanTimeoutSpin = new wxSpinCtrl(m_screenSharingPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 100, 10000, 1000);
    m_connectionTimeoutSpin = new wxSpinCtrl(m_screenSharingPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 100, 10000, 3000);
    sizer->Add(new wxStaticText(m_screenSharingPanel, wxID_ANY, wxT("Quality (%):")), 0, wxALL, 5);
    sizer->Add(m_qualitySpin, 0, wxALL, 5);
    sizer->Add(new wxStaticText(m_screenSharingPanel, wxID_ANY, wxT("Frame Rate (FPS):")), 0, wxALL, 5);
    sizer->Add(m_frameRateSpin, 0, wxALL, 5);
    sizer->Add(new wxStaticText(m_screenSharingPanel, wxID_ANY, wxT("Compression (%):")), 0, wxALL, 5);
    sizer->Add(m_compressionSpin, 0, wxALL, 5);
    sizer->Add(m_encryptionCheck, 0, wxALL, 5);
    sizer->Add(m_autoConnectCheck, 0, wxALL, 5);
    sizer->Add(new wxStaticText(m_screenSharingPanel, wxID_ANY, wxT("Scan Timeout (ms):")), 0, wxALL, 5);
    sizer->Add(m_scanTimeoutSpin, 0, wxALL, 5);
    sizer->Add(new wxStaticText(m_screenSharingPanel, wxID_ANY, wxT("Connection Timeout (ms):")), 0, wxALL, 5);
    sizer->Add(m_connectionTimeoutSpin, 0, wxALL, 5);
    m_screenSharingPanel->SetSizer(sizer);
    m_notebook->AddPage(m_screenSharingPanel, wxT("Screen Sharing"));
}

void SettingsPanel::CreateStealthPanel() {
    m_stealthPanel = new wxPanel(m_notebook);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    m_stealthModeCheck = new wxCheckBox(m_stealthPanel, wxID_ANY, wxT("Enable Stealth Mode"));
    m_examModeCheck = new wxCheckBox(m_stealthPanel, wxID_ANY, wxT("Enable Exam Mode"));
    m_remoteControlCheck = new wxCheckBox(m_stealthPanel, wxID_ANY, wxT("Enable Remote Control"));
    m_memoryProtectionCheck = new wxCheckBox(m_stealthPanel, wxID_ANY, wxT("Enable Memory Protection"));
    m_antiDetectionCheck = new wxCheckBox(m_stealthPanel, wxID_ANY, wxT("Enable Anti-Detection"));
    m_networkObfuscationCheck = new wxCheckBox(m_stealthPanel, wxID_ANY, wxT("Enable Network Obfuscation"));
    m_stealthLevelSpin = new wxSpinCtrl(m_stealthPanel, ID_STEALTH_LEVEL_SPIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10, 5);
    sizer->Add(m_stealthModeCheck, 0, wxALL, 5);
    sizer->Add(m_examModeCheck, 0, wxALL, 5);
    sizer->Add(m_remoteControlCheck, 0, wxALL, 5);
    sizer->Add(m_memoryProtectionCheck, 0, wxALL, 5);
    sizer->Add(m_antiDetectionCheck, 0, wxALL, 5);
    sizer->Add(m_networkObfuscationCheck, 0, wxALL, 5);
    sizer->Add(new wxStaticText(m_stealthPanel, wxID_ANY, wxT("Stealth Level:")), 0, wxALL, 5);
    sizer->Add(m_stealthLevelSpin, 0, wxALL, 5);
    m_stealthPanel->SetSizer(sizer);
    m_notebook->AddPage(m_stealthPanel, wxT("Stealth"));
}

void SettingsPanel::CreateAppearancePanel() {
    m_appearancePanel = new wxPanel(m_notebook);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    m_themeChoice = new wxChoice(m_appearancePanel, ID_THEME_CHOICE);
    m_themeChoice->Append(wxT("Light"));
    m_themeChoice->Append(wxT("Dark"));
    m_themeChoice->SetSelection(0);
    m_fullscreenCheck = new wxCheckBox(m_appearancePanel, wxID_ANY, wxT("Start in Fullscreen"));
    m_alwaysOnTopCheck = new wxCheckBox(m_appearancePanel, wxID_ANY, wxT("Always on Top"));
    m_showStatusBarCheck = new wxCheckBox(m_appearancePanel, wxID_ANY, wxT("Show Status Bar"));
    m_showToolBarCheck = new wxCheckBox(m_appearancePanel, wxID_ANY, wxT("Show Tool Bar"));
    m_opacitySpin = new wxSpinCtrl(m_appearancePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 20, 100, 100);
    sizer->Add(new wxStaticText(m_appearancePanel, wxID_ANY, wxT("Theme:")), 0, wxALL, 5);
    sizer->Add(m_themeChoice, 0, wxALL, 5);
    sizer->Add(m_fullscreenCheck, 0, wxALL, 5);
    sizer->Add(m_alwaysOnTopCheck, 0, wxALL, 5);
    sizer->Add(m_showStatusBarCheck, 0, wxALL, 5);
    sizer->Add(m_showToolBarCheck, 0, wxALL, 5);
    sizer->Add(new wxStaticText(m_appearancePanel, wxID_ANY, wxT("Opacity (%):")), 0, wxALL, 5);
    sizer->Add(m_opacitySpin, 0, wxALL, 5);
    m_appearancePanel->SetSizer(sizer);
    m_notebook->AddPage(m_appearancePanel, wxT("Appearance"));
}

void SettingsPanel::CreateSecurityPanel() {
    m_securityPanel = new wxPanel(m_notebook);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    m_encryptionEnabledCheck = new wxCheckBox(m_securityPanel, wxID_ANY, wxT("Enable Encryption"));
    m_compressionEnabledCheck = new wxCheckBox(m_securityPanel, wxID_ANY, wxT("Enable Compression"));
    m_autoLockCheck = new wxCheckBox(m_securityPanel, wxID_ANY, wxT("Enable Auto-Lock"));
    m_sessionTimeoutSpin = new wxSpinCtrl(m_securityPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1440, 30);
    m_maxLoginAttemptsSpin = new wxSpinCtrl(m_securityPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10, 3);
    sizer->Add(m_encryptionEnabledCheck, 0, wxALL, 5);
    sizer->Add(m_compressionEnabledCheck, 0, wxALL, 5);
    sizer->Add(m_autoLockCheck, 0, wxALL, 5);
    sizer->Add(new wxStaticText(m_securityPanel, wxID_ANY, wxT("Session Timeout (min):")), 0, wxALL, 5);
    sizer->Add(m_sessionTimeoutSpin, 0, wxALL, 5);
    sizer->Add(new wxStaticText(m_securityPanel, wxID_ANY, wxT("Max Login Attempts:")), 0, wxALL, 5);
    sizer->Add(m_maxLoginAttemptsSpin, 0, wxALL, 5);
    m_securityPanel->SetSizer(sizer);
    m_notebook->AddPage(m_securityPanel, wxT("Security"));
}

void SettingsPanel::CreateAdvancedPanel() {
    m_advancedPanel = new wxPanel(m_notebook);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    m_debugModeCheck = new wxCheckBox(m_advancedPanel, wxID_ANY, wxT("Enable Debug Mode"));
    m_loggingEnabledCheck = new wxCheckBox(m_advancedPanel, wxID_ANY, wxT("Enable Logging"));
    m_autoBackupCheck = new wxCheckBox(m_advancedPanel, wxID_ANY, wxT("Enable Auto-Backup"));
    m_backupIntervalSpin = new wxSpinCtrl(m_advancedPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1440, 60);
    m_logPathCtrl = new wxTextCtrl(m_advancedPanel, wxID_ANY, wxT("C:/Logs/masoudy.log"));
    sizer->Add(m_debugModeCheck, 0, wxALL, 5);
    sizer->Add(m_loggingEnabledCheck, 0, wxALL, 5);
    sizer->Add(m_autoBackupCheck, 0, wxALL, 5);
    sizer->Add(new wxStaticText(m_advancedPanel, wxID_ANY, wxT("Backup Interval (min):")), 0, wxALL, 5);
    sizer->Add(m_backupIntervalSpin, 0, wxALL, 5);
    sizer->Add(new wxStaticText(m_advancedPanel, wxID_ANY, wxT("Log Path:")), 0, wxALL, 5);
    sizer->Add(m_logPathCtrl, 0, wxALL | wxEXPAND, 5);
    m_advancedPanel->SetSizer(sizer);
    m_notebook->AddPage(m_advancedPanel, wxT("Advanced"));
}

// Event handlers
void SettingsPanel::OnApply(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    std::cout << "Apply settings requested" << std::endl;
    SaveSettings();
}

void SettingsPanel::OnReset(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    std::cout << "Reset settings requested" << std::endl;
    ResetToDefaults();
}

void SettingsPanel::OnExport(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    std::cout << "Export settings requested" << std::endl;
    ExportSettings("settings.ini");
}

void SettingsPanel::OnImport(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    std::cout << "Import settings requested" << std::endl;
    ImportSettings("settings.ini");
}

void SettingsPanel::OnGenerateKey(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    std::cout << "Generate key requested" << std::endl;
    wxString key = GenerateEncryptionKey();
    std::cout << "Generated key: " << key << std::endl;
}

void SettingsPanel::OnThemeChanged(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    std::cout << "Theme changed" << std::endl;
}

void SettingsPanel::OnStealthLevelChanged(wxSpinEvent& event) {
    (void)event; // Suppress unused parameter warning
    std::cout << "Stealth level changed" << std::endl;
}

wxString SettingsPanel::GenerateEncryptionKey() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    
    for (int i = 0; i < 32; ++i) {
        ss << std::setw(2) << dis(gen);
    }
    
    return wxString(ss.str());
} 