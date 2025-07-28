#include "screen_sharing_panel.h"
#include "masoudy_core.h"
#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/gauge.h>
#include <wx/spinctrl.h>
#include <wx/msgdlg.h>
#include <iostream>

// Event IDs
enum {
    ID_START_SHARING_BUTTON = 4000,
    ID_STOP_SHARING_BUTTON,
    ID_QUALITY_CHANGED,
    ID_FRAME_RATE_CHANGED,
    ID_COMPRESSION_CHANGED,
    ID_ENCRYPTION_TOGGLED,
    ID_REFRESH_BUTTON,
    ID_CLEAR_BUTTON
};

BEGIN_EVENT_TABLE(ScreenSharingPanel, wxPanel)
    EVT_BUTTON(ID_START_SHARING_BUTTON, ScreenSharingPanel::OnStartSharing)
    EVT_BUTTON(ID_STOP_SHARING_BUTTON, ScreenSharingPanel::OnStopSharing)
    EVT_BUTTON(ID_REFRESH_BUTTON, ScreenSharingPanel::OnRefresh)
    EVT_BUTTON(ID_CLEAR_BUTTON, ScreenSharingPanel::OnClear)
END_EVENT_TABLE()

ScreenSharingPanel::ScreenSharingPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY)
    , m_core(nullptr)
    , m_screenSharingActive(false)
    , m_quality(80)
    , m_frameRate(30)
    , m_compression(70)
    , m_encryptionEnabled(true) {
    
    std::cout << "ScreenSharingPanel constructor called" << std::endl;
    
    CreateControls();
}

ScreenSharingPanel::~ScreenSharingPanel() {
    std::cout << "ScreenSharingPanel destructor called" << std::endl;
}

void ScreenSharingPanel::SetCore(MasoudyCore::MasoudyCore* core) {
    m_core = core;
}

void ScreenSharingPanel::RefreshDisplay() {
    std::cout << "Refreshing screen sharing display" << std::endl;
}

void ScreenSharingPanel::UpdateStatus() {
    std::cout << "Updating screen sharing status" << std::endl;
}

void ScreenSharingPanel::ClearDisplay() {
    std::cout << "Clearing screen sharing display" << std::endl;
}

void ScreenSharingPanel::StartScreenSharing() {
    m_screenSharingActive = true;
    if (m_startButton) {
        m_startButton->Enable(false);
    }
    if (m_stopButton) {
        m_stopButton->Enable(true);
    }
    std::cout << "Screen sharing started" << std::endl;
}

void ScreenSharingPanel::StopScreenSharing() {
    m_screenSharingActive = false;
    if (m_startButton) {
        m_startButton->Enable(true);
    }
    if (m_stopButton) {
        m_stopButton->Enable(false);
    }
    std::cout << "Screen sharing stopped" << std::endl;
}

bool ScreenSharingPanel::IsScreenSharingActive() const {
    return m_screenSharingActive;
}

void ScreenSharingPanel::LoadSettings() {
    std::cout << "Loading screen sharing settings" << std::endl;
}

void ScreenSharingPanel::SaveSettings() {
    std::cout << "Saving screen sharing settings" << std::endl;
}

void ScreenSharingPanel::ResetToDefaults() {
    std::cout << "Resetting screen sharing settings to defaults" << std::endl;
}

void ScreenSharingPanel::CreateControls() {
    // Main sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    // Control section
    wxStaticBox* controlBox = new wxStaticBox(this, wxID_ANY, wxT("Screen Sharing Controls"));
    wxStaticBoxSizer* controlSizer = new wxStaticBoxSizer(controlBox, wxVERTICAL);
    // Start/Stop buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    m_startButton = new wxButton(this, ID_START_SHARING_BUTTON, wxT("Start Sharing"));
    m_stopButton = new wxButton(this, ID_STOP_SHARING_BUTTON, wxT("Stop Sharing"));
    m_stopButton->Enable(false);
    buttonSizer->Add(m_startButton, 0, wxALL, 5);
    buttonSizer->Add(m_stopButton, 0, wxALL, 5);
    controlSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);
    // Settings section
    wxStaticBox* settingsBox = new wxStaticBox(this, wxID_ANY, wxT("Settings"));
    wxStaticBoxSizer* settingsSizer = new wxStaticBoxSizer(settingsBox, wxVERTICAL);
    // Quality
    m_qualitySpin = new wxSpinCtrl(this, ID_QUALITY_CHANGED, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, m_quality);
    m_qualityLabel = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("Quality: %d%%"), m_quality));
    settingsSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Quality (%):")), 0, wxALL, 5);
    settingsSizer->Add(m_qualitySpin, 0, wxALL, 5);
    settingsSizer->Add(m_qualityLabel, 0, wxALL, 5);
    // Frame rate
    m_frameRateSpin = new wxSpinCtrl(this, ID_FRAME_RATE_CHANGED, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 60, m_frameRate);
    m_frameRateLabel = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("Frame Rate: %d FPS"), m_frameRate));
    settingsSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Frame Rate (FPS):")), 0, wxALL, 5);
    settingsSizer->Add(m_frameRateSpin, 0, wxALL, 5);
    settingsSizer->Add(m_frameRateLabel, 0, wxALL, 5);
    // Compression
    m_compressionSpin = new wxSpinCtrl(this, ID_COMPRESSION_CHANGED, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, m_compression);
    m_compressionLabel = new wxStaticText(this, wxID_ANY, wxString::Format(wxT("Compression: %d%%"), m_compression));
    settingsSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Compression (%):")), 0, wxALL, 5);
    settingsSizer->Add(m_compressionSpin, 0, wxALL, 5);
    settingsSizer->Add(m_compressionLabel, 0, wxALL, 5);
    // Encryption
    m_encryptionCheck = new wxCheckBox(this, ID_ENCRYPTION_TOGGLED, wxT("Enable Encryption"));
    m_encryptionCheck->SetValue(m_encryptionEnabled);
    settingsSizer->Add(m_encryptionCheck, 0, wxALL, 5);
    controlSizer->Add(settingsSizer, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(controlSizer, 0, wxEXPAND | wxALL, 5);
    // Progress section
    wxStaticBox* progressBox = new wxStaticBox(this, wxID_ANY, wxT("Progress"));
    wxStaticBoxSizer* progressSizer = new wxStaticBoxSizer(progressBox, wxVERTICAL);
    m_progressGauge = new wxGauge(this, wxID_ANY, 100);
    progressSizer->Add(m_progressGauge, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(progressSizer, 0, wxEXPAND | wxALL, 5);
    // Buttons
    wxBoxSizer* actionSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* refreshButton = new wxButton(this, ID_REFRESH_BUTTON, wxT("Refresh"));
    wxButton* clearButton = new wxButton(this, ID_CLEAR_BUTTON, wxT("Clear"));
    actionSizer->Add(refreshButton, 0, wxALL, 5);
    actionSizer->Add(clearButton, 0, wxALL, 5);
    mainSizer->Add(actionSizer, 0, wxALIGN_CENTER | wxALL, 5);
    // Status
    m_statusLabel = new wxStaticText(this, wxID_ANY, wxT("Screen Sharing Panel Ready"));
    mainSizer->Add(m_statusLabel, 0, wxALIGN_CENTER | wxALL, 5);
    SetSizer(mainSizer);
    // Bind spin events
    m_qualitySpin->Bind(wxEVT_SPINCTRL, &ScreenSharingPanel::OnQualityChanged, this);
    m_frameRateSpin->Bind(wxEVT_SPINCTRL, &ScreenSharingPanel::OnFrameRateChanged, this);
    m_compressionSpin->Bind(wxEVT_SPINCTRL, &ScreenSharingPanel::OnCompressionChanged, this);
    m_encryptionCheck->Bind(wxEVT_CHECKBOX, &ScreenSharingPanel::OnEncryptionToggled, this);
}

// Event handlers
void ScreenSharingPanel::OnStartSharing(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    
    if (!m_core) {
        wxMessageBox("Core engine not initialized", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    if (m_screenSharingActive) {
        wxMessageBox("Screen sharing is already active", "Warning", wxOK | wxICON_WARNING);
        return;
    }
    
    // Get current settings from UI
    m_quality = m_qualitySpin ? m_qualitySpin->GetValue() : 80;
    m_frameRate = m_frameRateSpin ? m_frameRateSpin->GetValue() : 30;
    m_compression = m_compressionSpin ? m_compressionSpin->GetValue() : 70;
    m_encryptionEnabled = m_encryptionCheck ? m_encryptionCheck->GetValue() : true;
    
    // Configure screen sharing settings
    MasoudyCore::ScreenSharingSettings settings;
    settings.quality = m_quality;
    settings.frameRate = m_frameRate;
    settings.compression = m_compression;
    settings.encryption = m_encryptionEnabled;
    settings.remoteOnly = true;
    settings.examMode = false; // Will be set by main frame
    
    // Start screen sharing
    if (m_core->StartScreenSharing(settings)) {
        StartScreenSharing();
        UpdateStatus();
        
        // Update status text
        if (m_statusLabel) {
            m_statusLabel->SetLabel("Screen sharing active");
        }
        
        wxMessageBox("Screen sharing started successfully", "Screen Sharing", wxOK | wxICON_INFORMATION);
    } else {
        wxMessageBox("Failed to start screen sharing", "Error", wxOK | wxICON_ERROR);
    }
}

void ScreenSharingPanel::OnStopSharing(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    
    if (!m_core) {
        wxMessageBox("Core engine not initialized", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    if (!m_screenSharingActive) {
        wxMessageBox("Screen sharing is not active", "Warning", wxOK | wxICON_WARNING);
        return;
    }
    
    // Stop screen sharing
    if (m_core->StopScreenSharing()) {
        StopScreenSharing();
        UpdateStatus();
        
        // Update status text
        if (m_statusLabel) {
            m_statusLabel->SetLabel("Screen sharing inactive");
        }
        
        wxMessageBox("Screen sharing stopped successfully", "Screen Sharing", wxOK | wxICON_INFORMATION);
    } else {
        wxMessageBox("Failed to stop screen sharing", "Error", wxOK | wxICON_ERROR);
    }
}

void ScreenSharingPanel::OnQualityChanged(wxSpinEvent& event) {
    (void)event; // Suppress unused parameter warning
    
    if (m_qualitySpin) {
        m_quality = m_qualitySpin->GetValue();
        
        // Update quality label
        if (m_qualityLabel) {
            m_qualityLabel->SetLabel(wxString::Format("Quality: %d%%", m_quality));
        }
        
        // Apply to core if active
        if (m_core && m_screenSharingActive) {
            // Note: In a real implementation, you'd update the core settings
            // For now, we'll just update the UI
        }
    }
}

void ScreenSharingPanel::OnFrameRateChanged(wxSpinEvent& event) {
    (void)event; // Suppress unused parameter warning
    
    if (m_frameRateSpin) {
        m_frameRate = m_frameRateSpin->GetValue();
        
        // Update frame rate label
        if (m_frameRateLabel) {
            m_frameRateLabel->SetLabel(wxString::Format("Frame Rate: %d fps", m_frameRate));
        }
        
        // Apply to core if active
        if (m_core && m_screenSharingActive) {
            // Note: In a real implementation, you'd update the core settings
            // For now, we'll just update the UI
        }
    }
}

void ScreenSharingPanel::OnCompressionChanged(wxSpinEvent& event) {
    (void)event; // Suppress unused parameter warning
    
    if (m_compressionSpin) {
        m_compression = m_compressionSpin->GetValue();
        
        // Update compression label
        if (m_compressionLabel) {
            m_compressionLabel->SetLabel(wxString::Format("Compression: %d%%", m_compression));
        }
        
        // Apply to core if active
        if (m_core && m_screenSharingActive) {
            // Note: In a real implementation, you'd update the core settings
            // For now, we'll just update the UI
        }
    }
}

void ScreenSharingPanel::OnEncryptionToggled(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    
    if (m_encryptionCheck) {
        m_encryptionEnabled = m_encryptionCheck->GetValue();
        
        // Apply to core if active
        if (m_core && m_screenSharingActive) {
            // Note: In a real implementation, you'd update the core settings
            // For now, we'll just update the UI
        }
    }
}

void ScreenSharingPanel::OnRefresh(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    std::cout << "Refresh requested" << std::endl;
    RefreshDisplay();
}

void ScreenSharingPanel::OnClear(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    std::cout << "Clear requested" << std::endl;
    ClearDisplay();
} 