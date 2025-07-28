#pragma once
#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/notebook.h>
#include <wx/statusbr.h>
#include <wx/menu.h>
#include <wx/accel.h>

// Forward declarations
namespace MasoudyCore {
    class MasoudyCore;
    enum class ConnectionStatus;
}

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);
    virtual ~MainFrame();

    // Event handlers
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnExamMode(wxCommandEvent& event);
    void OnStealthMode(wxCommandEvent& event);
    void OnHideApp(wxCommandEvent& event);
    void OnNetworkScanner(wxCommandEvent& event);
    void OnScreenSharing(wxCommandEvent& event);
    void OnToggleConnection(wxCommandEvent& event);
    void OnEnableRemoteControl(wxCommandEvent& event);
    void OnDisableRemoteControl(wxCommandEvent& event);
    void OnScanNetwork(wxCommandEvent& event);
    void OnConnectTarget(wxCommandEvent& event);
    void OnDisconnect(wxCommandEvent& event);
    void OnStartScreenSharing(wxCommandEvent& event);
    void OnStopScreenSharing(wxCommandEvent& event);
    void OnSaveSettings(wxCommandEvent& event);
    void OnLoadSettings(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

    // Status updates
    void UpdateConnectionStatus(MasoudyCore::ConnectionStatus status);
    void UpdateScreenSharingStatus(bool active);
    void SetExamMode(bool examMode);
    void SetRemoteControlEnabled(bool enabled);

private:
    void CreateMenuBar();
    void CreateStatusBarImpl();
    void CreateNotebook();
    void CreateAcceleratorTable();

    // UI components
    wxNotebook* m_notebook;
    wxStatusBar* m_statusBar;
    wxMenuBar* m_menuBar;
    wxAcceleratorTable* m_accelTable;

    // Application state
    MasoudyCore::MasoudyCore* m_core; // Will be used when core integration is complete
    bool m_examMode;
    bool m_remoteControlEnabled;
    bool m_stealthModeActive;

    DECLARE_EVENT_TABLE()
}; 