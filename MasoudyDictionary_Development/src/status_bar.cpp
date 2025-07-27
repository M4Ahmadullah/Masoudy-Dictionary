#include "status_bar.h"
#include "masoudy_core.h"
#include <wx/wx.h>
#include <wx/statusbr.h>
#include <wx/gauge.h>
#include <wx/stattext.h>
#include <wx/sizer.h>

StatusBar::StatusBar(wxWindow* parent)
    : wxStatusBar(parent, wxID_ANY, wxST_SIZEGRIP) {
    
    CreateFields();
}

StatusBar::~StatusBar() {
}

void StatusBar::UpdateState(MasoudyCore::AppMode mode, bool stealthMode) {
    SetModeStatus(mode);
    SetConnectionStatus(MasoudyCore::ConnectionStatus::DISCONNECTED);
}

void StatusBar::SetProgress(int progress) {
    if (m_progressGauge) {
        m_progressGauge->SetValue(progress);
    }
}

void StatusBar::SetConnectionStatus(MasoudyCore::ConnectionStatus status) {
    UpdateConnectionStatusText(status);
}

void StatusBar::SetModeStatus(MasoudyCore::AppMode mode) {
    UpdateModeStatusText(mode);
}

void StatusBar::CreateFields() {
    // Create status fields
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    
    // Progress gauge
    m_progressGauge = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxSize(100, 20));
    sizer->Add(m_progressGauge, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    
    // Connection status
    m_connectionStatusLabel = new wxStaticText(this, wxID_ANY, "Disconnected");
    sizer->Add(m_connectionStatusLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    
    // Mode status
    m_modeStatusLabel = new wxStaticText(this, wxID_ANY, "Normal Mode");
    sizer->Add(m_modeStatusLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    
    SetSizer(sizer);
}

void StatusBar::UpdateConnectionStatusText(MasoudyCore::ConnectionStatus status) {
    if (m_connectionStatusLabel) {
        wxString statusText;
        switch (status) {
            case MasoudyCore::ConnectionStatus::CONNECTED:
                statusText = "Connected";
                break;
            case MasoudyCore::ConnectionStatus::CONNECTING:
                statusText = "Connecting...";
                break;
            case MasoudyCore::ConnectionStatus::DISCONNECTED:
                statusText = "Disconnected";
                break;
            case MasoudyCore::ConnectionStatus::ERROR:
                statusText = "Error";
                break;
            default:
                statusText = "Unknown";
                break;
        }
        m_connectionStatusLabel->SetLabel(statusText);
    }
}

void StatusBar::UpdateModeStatusText(MasoudyCore::AppMode mode) {
    if (m_modeStatusLabel) {
        wxString modeText;
        switch (mode) {
            case MasoudyCore::AppMode::STEALTH_MODE:
                modeText = "Stealth Mode";
                break;
            case MasoudyCore::AppMode::SCREEN_SHARING:
                modeText = "Screen Sharing Mode";
                break;
            case MasoudyCore::AppMode::RESEARCH_MODE:
                modeText = "Research Mode";
                break;
            default:
                modeText = "Normal Mode";
                break;
        }
        m_modeStatusLabel->SetLabel(modeText);
    }
} 