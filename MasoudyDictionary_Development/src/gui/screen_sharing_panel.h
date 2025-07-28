#pragma once

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/gauge.h>
#include <wx/spinctrl.h>
#include <wx/spinbutt.h>

// Forward declaration
namespace MasoudyCore {
    class MasoudyCore;
}

class ScreenSharingPanel : public wxPanel {
public:
    ScreenSharingPanel(wxWindow* parent);
    virtual ~ScreenSharingPanel();

    // Core integration
    void SetCore(MasoudyCore::MasoudyCore* core);

    // UI operations
    void RefreshDisplay();
    void UpdateStatus();
    void ClearDisplay();

    // Screen sharing control
    void StartScreenSharing();
    void StopScreenSharing();
    bool IsScreenSharingActive() const;

    // Settings
    void LoadSettings();
    void SaveSettings();
    void ResetToDefaults();

private:
    // UI creation
    void CreateControls();

    // Event handlers
    void OnStartSharing(wxCommandEvent& event);
    void OnStopSharing(wxCommandEvent& event);
    void OnQualityChanged(wxSpinEvent& event);
    void OnFrameRateChanged(wxSpinEvent& event);
    void OnCompressionChanged(wxSpinEvent& event);
    void OnEncryptionToggled(wxCommandEvent& event);
    void OnRefresh(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);

    // Core reference
    MasoudyCore::MasoudyCore* m_core;

    // UI components
    wxButton* m_startButton;
    wxButton* m_stopButton;
    wxCheckBox* m_encryptionCheck;
    wxStaticText* m_qualityLabel;
    wxStaticText* m_frameRateLabel;
    wxStaticText* m_compressionLabel;
    wxStaticText* m_statusLabel;
    wxGauge* m_progressGauge;
    wxSpinCtrl* m_qualitySpin;
    wxSpinCtrl* m_frameRateSpin;
    wxSpinCtrl* m_compressionSpin;

    // State
    bool m_screenSharingActive;
    int m_quality;
    int m_frameRate;
    int m_compression;
    bool m_encryptionEnabled;

    DECLARE_EVENT_TABLE()
}; 