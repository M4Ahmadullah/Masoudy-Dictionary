#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include <wx/wx.h>
#include <wx/statusbr.h>
#include <wx/gauge.h>
#include <wx/stattext.h>
#include <wx/sizer.h>

// Forward declarations
namespace MasoudyCore {
    enum class AppMode;
    enum class ConnectionStatus;
}

class StatusBar : public wxStatusBar {
public:
    StatusBar(wxWindow* parent);
    virtual ~StatusBar();

    // Status bar operations
    void UpdateState(MasoudyCore::AppMode mode, bool stealthMode);
    void SetProgress(int progress);
    void SetConnectionStatus(MasoudyCore::ConnectionStatus status);
    void SetModeStatus(MasoudyCore::AppMode mode);

private:
    // Status fields
    wxGauge* m_progressGauge;
    wxStaticText* m_connectionStatusLabel;
    wxStaticText* m_modeStatusLabel;
    
    // Helper methods
    void CreateFields();
    void UpdateConnectionStatusText(MasoudyCore::ConnectionStatus status);
    void UpdateModeStatusText(MasoudyCore::AppMode mode);
};

#endif // STATUS_BAR_H 