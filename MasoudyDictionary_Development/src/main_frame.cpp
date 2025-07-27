#include "main_frame.h"
#include "masoudy_core.h"
#include "network_scanner_dialog.h"
#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/statusbr.h>
#include <wx/menu.h>
#include <wx/accel.h>
#include <wx/msgdlg.h>

// Event IDs
enum {
    ID_HOTKEY_EXAM_MODE = 2000,
    ID_HOTKEY_STEALTH_MODE,
    ID_HOTKEY_HIDE_APP,
    ID_HOTKEY_NETWORK_SCANNER,
    ID_HOTKEY_SCREEN_SHARING,
    ID_HOTKEY_EXIT_APP,
    ID_HOTKEY_TOGGLE_CONNECTION,
    ID_ENABLE_REMOTE_CONTROL,
    ID_DISABLE_REMOTE_CONTROL,
    ID_SCAN_NETWORK,
    ID_CONNECT_TARGET,
    ID_DISCONNECT,
    ID_START_SCREEN_SHARING,
    ID_STOP_SCREEN_SHARING,
    ID_SAVE_SETTINGS,
    ID_LOAD_SETTINGS,
    ID_STEALTH_SETTINGS
};

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
    EVT_MENU(ID_HOTKEY_EXAM_MODE, MainFrame::OnExamMode)
    EVT_MENU(ID_HOTKEY_STEALTH_MODE, MainFrame::OnStealthMode)
    EVT_MENU(ID_HOTKEY_HIDE_APP, MainFrame::OnHideApp)
    EVT_MENU(ID_HOTKEY_NETWORK_SCANNER, MainFrame::OnNetworkScanner)
    EVT_MENU(ID_HOTKEY_SCREEN_SHARING, MainFrame::OnScreenSharing)
    EVT_MENU(ID_HOTKEY_TOGGLE_CONNECTION, MainFrame::OnToggleConnection)
    EVT_MENU(ID_ENABLE_REMOTE_CONTROL, MainFrame::OnEnableRemoteControl)
    EVT_MENU(ID_DISABLE_REMOTE_CONTROL, MainFrame::OnDisableRemoteControl)
    EVT_MENU(ID_SCAN_NETWORK, MainFrame::OnScanNetwork)
    EVT_MENU(ID_CONNECT_TARGET, MainFrame::OnConnectTarget)
    EVT_MENU(ID_DISCONNECT, MainFrame::OnDisconnect)
    EVT_MENU(ID_START_SCREEN_SHARING, MainFrame::OnStartScreenSharing)
    EVT_MENU(ID_STOP_SCREEN_SHARING, MainFrame::OnStopScreenSharing)
    EVT_MENU(ID_SAVE_SETTINGS, MainFrame::OnSaveSettings)
    EVT_MENU(ID_LOAD_SETTINGS, MainFrame::OnLoadSettings)
    EVT_CLOSE(MainFrame::OnClose)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1200, 800))
    , m_core(nullptr)
    , m_examMode(false)
    , m_remoteControlEnabled(false)
    , m_stealthModeActive(false) {
    
    // Initialize core engine
    m_core = new MasoudyCore::MasoudyCore();
    if (!m_core->Initialize(MasoudyCore::AppMode::STEALTH_MODE)) {
        wxMessageBox("Failed to initialize core engine", "Error", wxOK | wxICON_ERROR);
    }
    
    // Create menu bar
    CreateMenuBar();
    
    // Create status bar
    CreateStatusBarImpl();
    
    // Create notebook
    CreateNotebook();
    
    // Create accelerator table
    CreateAcceleratorTable();
    
    // Set window properties
    SetMinSize(wxSize(800, 600));
    Centre();
}

MainFrame::~MainFrame() {
    if (m_core) {
        m_core->Shutdown();
        delete m_core;
        m_core = nullptr;
    }
}

void MainFrame::CreateMenuBar() {
    m_menuBar = new wxMenuBar();
    
    // File menu
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(wxID_EXIT, "E&xit\tCtrl+Q", "Exit the application");
    m_menuBar->Append(fileMenu, "&File");
    
    // Tools menu
    wxMenu* toolsMenu = new wxMenu();
    toolsMenu->Append(ID_HOTKEY_NETWORK_SCANNER, "&Network Scanner\tCtrl+N", "Open network scanner");
    toolsMenu->Append(ID_HOTKEY_SCREEN_SHARING, "&Screen Sharing\tCtrl+S", "Start screen sharing");
    toolsMenu->AppendSeparator();
    toolsMenu->Append(ID_HOTKEY_TOGGLE_CONNECTION, "&Toggle Connection\tCtrl+T", "Toggle connection");
    m_menuBar->Append(toolsMenu, "&Tools");
    
    // Mode menu
    wxMenu* modeMenu = new wxMenu();
    modeMenu->AppendCheckItem(ID_HOTKEY_EXAM_MODE, "&Exam Mode\tCtrl+E", "Enable exam mode");
    modeMenu->AppendCheckItem(ID_HOTKEY_STEALTH_MODE, "&Stealth Mode\tCtrl+H", "Enable stealth mode");
    m_menuBar->Append(modeMenu, "&Mode");
    
    // Help menu
    wxMenu* helpMenu = new wxMenu();
    helpMenu->Append(wxID_ABOUT, "&About\tF1", "About this application");
    m_menuBar->Append(helpMenu, "&Help");
    
    SetMenuBar(m_menuBar);
}

void MainFrame::CreateStatusBarImpl() {
    CreateStatusBar();
    m_statusBar = GetStatusBar();
    m_statusBar->SetStatusText("Ready", 0);
    m_statusBar->SetStatusText("Disconnected", 1);
    m_statusBar->SetStatusText("Normal Mode", 2);
}

void MainFrame::CreateNotebook() {
    m_notebook = new wxNotebook(this, wxID_ANY);
    
    // Create panels (placeholder for now)
    wxPanel* researchPanel = new wxPanel(m_notebook);
    wxPanel* networkPanel = new wxPanel(m_notebook);
    wxPanel* settingsPanel = new wxPanel(m_notebook);
    
    m_notebook->AddPage(researchPanel, "Research", true);
    m_notebook->AddPage(networkPanel, "Network", false);
    m_notebook->AddPage(settingsPanel, "Settings", false);
}

void MainFrame::CreateAcceleratorTable() {
    wxAcceleratorEntry entries[] = {
        wxAcceleratorEntry(wxACCEL_CTRL, 'Q', wxID_EXIT),
        wxAcceleratorEntry(wxACCEL_CTRL, 'E', ID_HOTKEY_EXAM_MODE),
        wxAcceleratorEntry(wxACCEL_CTRL, 'H', ID_HOTKEY_STEALTH_MODE),
        wxAcceleratorEntry(wxACCEL_CTRL, 'N', ID_HOTKEY_NETWORK_SCANNER),
        wxAcceleratorEntry(wxACCEL_CTRL, 'S', ID_HOTKEY_SCREEN_SHARING),
        wxAcceleratorEntry(wxACCEL_CTRL, 'T', ID_HOTKEY_TOGGLE_CONNECTION)
    };
    
    m_accelTable = new wxAcceleratorTable(WXSIZEOF(entries), entries);
    SetAcceleratorTable(*m_accelTable);
}

void MainFrame::OnExit(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    Close();
}

void MainFrame::OnAbout(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    wxMessageBox("Masoudy Dictionary v1.0.0\nAdvanced Exam-Proof Cybersecurity Application",
                "About Masoudy Dictionary", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnExamMode(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    
    if (!m_core) {
        wxMessageBox("Core engine not initialized", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    m_examMode = !m_examMode;
    
    if (m_examMode) {
        // Enable exam mode - hide application completely
        if (m_core->EnableExamMode()) {
            SetExamMode(m_examMode);
            m_statusBar->SetStatusText("Exam Mode", 2);
            
            // Hide the application window
            Hide();
            
            wxMessageBox("Exam mode enabled. Application is now hidden.\nScreen sharing continues in background.", 
                        "Exam Mode", wxOK | wxICON_INFORMATION);
        } else {
            m_examMode = false;
            wxMessageBox("Failed to enable exam mode", "Error", wxOK | wxICON_ERROR);
        }
    } else {
        // Disable exam mode - show application again
        if (m_core->DisableExamMode()) {
            SetExamMode(m_examMode);
            m_statusBar->SetStatusText("Normal Mode", 2);
            
            // Show the application window
            Show();
            
            wxMessageBox("Exam mode disabled. Application is now visible.", 
                        "Exam Mode", wxOK | wxICON_INFORMATION);
        } else {
            m_examMode = true;
            wxMessageBox("Failed to disable exam mode", "Error", wxOK | wxICON_ERROR);
        }
    }
}

void MainFrame::OnStealthMode(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    
    if (!m_core) {
        wxMessageBox("Core engine not initialized", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    m_stealthModeActive = !m_stealthModeActive;
    
    if (m_stealthModeActive) {
        // Enable stealth mode - hide from all monitoring tools
        if (m_core->EnableStealthMode()) {
            m_statusBar->SetStatusText("Stealth Mode", 2);
            
            // Apply all stealth techniques
            m_core->HideFromTaskManager();
            m_core->HideFromProcessMonitor();
            m_core->HideFromWireshark();
            m_core->HideFromAntivirus();
            
            wxMessageBox("Stealth mode enabled.\nProcess hidden from monitoring tools.", 
                        "Stealth Mode", wxOK | wxICON_INFORMATION);
        } else {
            m_stealthModeActive = false;
            wxMessageBox("Failed to enable stealth mode", "Error", wxOK | wxICON_ERROR);
        }
    } else {
        // Disable stealth mode
        if (m_core->DisableStealthMode()) {
            m_statusBar->SetStatusText("Normal Mode", 2);
            
            wxMessageBox("Stealth mode disabled.\nProcess visible to monitoring tools.", 
                        "Stealth Mode", wxOK | wxICON_INFORMATION);
        } else {
            m_stealthModeActive = true;
            wxMessageBox("Failed to disable stealth mode", "Error", wxOK | wxICON_ERROR);
        }
    }
}

void MainFrame::OnHideApp(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    Hide();
}

void MainFrame::OnNetworkScanner(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    
    // Create and show network scanner dialog
    NetworkScannerDialog dialog(this);
    if (dialog.ShowModal() == wxID_OK) {
        // Get discovered hosts
        std::vector<MasoudyCore::NetworkHost> hosts = dialog.GetDiscoveredHosts();
        
        if (!hosts.empty()) {
            wxString message = "Discovered hosts:\n";
            for (const auto& host : hosts) {
                message += wxString::Format("- %s (%s)\n", host.ip, host.hostname);
            }
            wxMessageBox(message, "Network Scan Results", wxOK | wxICON_INFORMATION);
        } else {
            wxMessageBox("No hosts discovered", "Network Scan Results", wxOK | wxICON_WARNING);
        }
    }
}

void MainFrame::OnScreenSharing(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    
    if (!m_core) {
        wxMessageBox("Core engine not initialized", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    // Toggle screen sharing
    if (m_core->IsScreenSharingActive()) {
        if (m_core->StopScreenSharing()) {
            UpdateScreenSharingStatus(false);
            wxMessageBox("Screen sharing stopped", "Screen Sharing", wxOK | wxICON_INFORMATION);
        } else {
            wxMessageBox("Failed to stop screen sharing", "Error", wxOK | wxICON_ERROR);
        }
    } else {
        // Start screen sharing
        MasoudyCore::ScreenSharingSettings settings;
        settings.quality = 80;
        settings.frameRate = 15;
        settings.compression = 70;
        settings.encryption = true;
        settings.remoteOnly = true;
        settings.examMode = m_examMode;
        
        if (m_core->StartScreenSharing(settings)) {
            UpdateScreenSharingStatus(true);
            wxMessageBox("Screen sharing started", "Screen Sharing", wxOK | wxICON_INFORMATION);
        } else {
            wxMessageBox("Failed to start screen sharing", "Error", wxOK | wxICON_ERROR);
        }
    }
}

void MainFrame::OnToggleConnection(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    
    if (!m_core) {
        wxMessageBox("Core engine not initialized", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    // Toggle connection
    if (m_core->GetConnectionStatus() == MasoudyCore::ConnectionStatus::CONNECTED) {
        // Disconnect - stop screen sharing which will disconnect
        if (m_core->StopScreenSharing()) {
            UpdateConnectionStatus(MasoudyCore::ConnectionStatus::DISCONNECTED);
            wxMessageBox("Disconnected", "Connection", wxOK | wxICON_INFORMATION);
        } else {
            wxMessageBox("Failed to disconnect", "Error", wxOK | wxICON_ERROR);
        }
    } else {
        // Connect - show connection dialog
        wxString target = wxGetTextFromUser("Enter target IP:port", "Connect to Target", "192.168.1.100:8080");
        if (!target.IsEmpty()) {
            if (m_core->ConnectToTarget(target.ToStdString())) {
                UpdateConnectionStatus(MasoudyCore::ConnectionStatus::CONNECTED);
                wxMessageBox("Connected to " + target, "Connection", wxOK | wxICON_INFORMATION);
            } else {
                wxMessageBox("Failed to connect to " + target, "Error", wxOK | wxICON_ERROR);
            }
        }
    }
}

void MainFrame::OnEnableRemoteControl(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    
    if (!m_core) {
        wxMessageBox("Core engine not initialized", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    if (m_core->EnableRemoteControl()) {
        m_remoteControlEnabled = true;
        SetRemoteControlEnabled(true);
        wxMessageBox("Remote control enabled.\nThis system can now be controlled remotely.", 
                    "Remote Control", wxOK | wxICON_INFORMATION);
    } else {
        wxMessageBox("Failed to enable remote control", "Error", wxOK | wxICON_ERROR);
    }
}

void MainFrame::OnDisableRemoteControl(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    
    if (!m_core) {
        wxMessageBox("Core engine not initialized", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    if (m_core->DisableRemoteControl()) {
        m_remoteControlEnabled = false;
        SetRemoteControlEnabled(false);
        wxMessageBox("Remote control disabled.\nThis system can no longer be controlled remotely.", 
                    "Remote Control", wxOK | wxICON_INFORMATION);
    } else {
        wxMessageBox("Failed to disable remote control", "Error", wxOK | wxICON_ERROR);
    }
}

void MainFrame::OnScanNetwork(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    wxMessageBox("Network Scan - Feature coming soon!", "Network Scan", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnConnectTarget(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    wxMessageBox("Connect to Target - Feature coming soon!", "Connection", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnDisconnect(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    wxMessageBox("Disconnect - Feature coming soon!", "Connection", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnStartScreenSharing(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    wxMessageBox("Start Screen Sharing - Feature coming soon!", "Screen Sharing", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnStopScreenSharing(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    wxMessageBox("Stop Screen Sharing - Feature coming soon!", "Screen Sharing", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnSaveSettings(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    wxMessageBox("Save Settings - Feature coming soon!", "Settings", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnLoadSettings(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    wxMessageBox("Load Settings - Feature coming soon!", "Settings", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnClose(wxCloseEvent& event) {
    (void)event; // Suppress unused parameter warning
    Destroy();
}

void MainFrame::UpdateConnectionStatus(MasoudyCore::ConnectionStatus status) {
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
    m_statusBar->SetStatusText(statusText, 1);
}

void MainFrame::UpdateScreenSharingStatus(bool active) {
    m_statusBar->SetStatusText(active ? "Screen Sharing Active" : "Screen Sharing Inactive", 0);
}

void MainFrame::SetExamMode(bool examMode) {
    m_examMode = examMode;
}

void MainFrame::SetRemoteControlEnabled(bool enabled) {
    m_remoteControlEnabled = enabled;
} 