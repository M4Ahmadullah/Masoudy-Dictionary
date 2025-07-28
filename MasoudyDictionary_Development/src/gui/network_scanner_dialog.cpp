#include "network_scanner_dialog.h"
#include "masoudy_core.h"
#include "network_scanner.h"
#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/listbox.h>
#include <wx/choice.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/msgdlg.h>

// Event IDs
enum {
    ID_START_SCAN = 3000,
    ID_STOP_SCAN,
    ID_CONNECT,
    ID_CANCEL,
    ID_MANUAL_ENTRY,
    ID_UPDATE_TIMER // Added for the update timer
};

BEGIN_EVENT_TABLE(NetworkScannerDialog, wxDialog)
    EVT_BUTTON(ID_START_SCAN, NetworkScannerDialog::OnStartScan)
    EVT_BUTTON(ID_STOP_SCAN, NetworkScannerDialog::OnStopScan)
    EVT_BUTTON(ID_CONNECT, NetworkScannerDialog::OnConnect)
    EVT_BUTTON(ID_CANCEL, NetworkScannerDialog::OnCancel)
    EVT_BUTTON(ID_MANUAL_ENTRY, NetworkScannerDialog::OnManualEntry)
    EVT_TIMER(ID_UPDATE_TIMER, NetworkScannerDialog::OnUpdateTimer) // Added for the update timer
END_EVENT_TABLE()

NetworkScannerDialog::NetworkScannerDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, wxT("Network Scanner"), wxDefaultPosition, wxSize(600, 400))
    , m_scanning(false) {
    
    CreateControls();
    Layout();
    Centre();
}

NetworkScannerDialog::~NetworkScannerDialog() {
}

void NetworkScannerDialog::CreateControls() {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Title
    wxStaticText* titleLabel = new wxStaticText(this, wxID_ANY, 
        wxT("Network Scanner\nDiscover and connect to network hosts"));
    mainSizer->Add(titleLabel, 0, wxALL | wxALIGN_CENTER, 10);
    
    // Scan settings
    wxStaticBox* settingsBox = new wxStaticBox(this, wxID_ANY, wxT("Scan Settings"));
    wxStaticBoxSizer* settingsSizer = new wxStaticBoxSizer(settingsBox, wxVERTICAL);
    
    // IP range
    wxStaticText* ipLabel = new wxStaticText(this, wxID_ANY, wxT("IP Range:"));
    settingsSizer->Add(ipLabel, 0, wxALL, 5);
    
    m_ipRangeCtrl = new wxTextCtrl(this, wxID_ANY, wxT("192.168.1.1-254"));
    settingsSizer->Add(m_ipRangeCtrl, 0, wxEXPAND | wxALL, 5);
    
    // Port range
    wxStaticText* portLabel = new wxStaticText(this, wxID_ANY, wxT("Port Range:"));
    settingsSizer->Add(portLabel, 0, wxALL, 5);
    
    m_portRangeCtrl = new wxTextCtrl(this, wxID_ANY, wxT("80,443,22,3389"));
    settingsSizer->Add(m_portRangeCtrl, 0, wxEXPAND | wxALL, 5);
    
    mainSizer->Add(settingsSizer, 0, wxEXPAND | wxALL, 5);
    
    // Results
    wxStaticBox* resultsBox = new wxStaticBox(this, wxID_ANY, wxT("Discovered Hosts"));
    wxStaticBoxSizer* resultsSizer = new wxStaticBoxSizer(resultsBox, wxVERTICAL);
    
    m_hostsList = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(400, 200));
    resultsSizer->Add(m_hostsList, 1, wxEXPAND | wxALL, 5);
    
    mainSizer->Add(resultsSizer, 1, wxEXPAND | wxALL, 5);
    
    // Buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    
    m_startScanButton = new wxButton(this, ID_START_SCAN, wxT("Start Scan"));
    buttonSizer->Add(m_startScanButton, 0, wxALL, 5);
    
    m_stopScanButton = new wxButton(this, ID_STOP_SCAN, wxT("Stop Scan"));
    m_stopScanButton->Enable(false);
    buttonSizer->Add(m_stopScanButton, 0, wxALL, 5);
    
    m_connectButton = new wxButton(this, ID_CONNECT, wxT("Connect"));
    m_connectButton->Enable(false);
    buttonSizer->Add(m_connectButton, 0, wxALL, 5);
    
    m_manualEntryButton = new wxButton(this, ID_MANUAL_ENTRY, wxT("Manual Entry"));
    buttonSizer->Add(m_manualEntryButton, 0, wxALL, 5);
    
    m_cancelButton = new wxButton(this, ID_CANCEL, wxT("Cancel"));
    buttonSizer->Add(m_cancelButton, 0, wxALL, 5);
    
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);
    
    SetSizer(mainSizer);
}

std::vector<MasoudyCore::NetworkHost> NetworkScannerDialog::GetDiscoveredHosts() const {
    if (m_networkScanner) {
        return m_networkScanner->GetDiscoveredHosts();
    }
    return std::vector<MasoudyCore::NetworkHost>();
}

void NetworkScannerDialog::SetDiscoveredHosts(const std::vector<MasoudyCore::NetworkHost>& hosts) {
    m_discoveredHosts = hosts;
    UpdateHostsList();
}

void NetworkScannerDialog::UpdateHostsList() {
    if (!m_networkScanner) {
        return;
    }
    
    // Get discovered hosts
    std::vector<MasoudyCore::NetworkHost> hosts = m_networkScanner->GetDiscoveredHosts();
    
    // Clear and repopulate list
    m_hostsList->Clear();
    
    for (const auto& host : hosts) {
        wxString hostInfo = wxString::Format("%s (%s) - %s", 
                                           host.ip, host.hostname, host.os);
        m_hostsList->Append(hostInfo);
    }
    
    // Enable connect button if hosts found
    m_connectButton->Enable(!hosts.empty());
}

void NetworkScannerDialog::OnStartScan(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    
    if (m_scanning) {
        return;
    }
    
    // Parse IP range
    wxString ipRange = m_ipRangeCtrl->GetValue();
    if (ipRange.IsEmpty()) {
        wxMessageBox("Please enter an IP range", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    // Parse the IP range (e.g., "192.168.1.1-254")
    wxString baseIP;
    int startIP = 1, endIP = 254;
    
    if (ipRange.Contains("-")) {
        wxString startStr = ipRange.BeforeFirst('-');
        wxString endStr = ipRange.AfterFirst('-');
        
        // Extract base IP (e.g., "192.168.1" from "192.168.1.1")
        if (startStr.Contains(".")) {
            baseIP = startStr.BeforeLast('.');
            startIP = wxAtoi(startStr.AfterLast('.'));
            endIP = wxAtoi(endStr);
        }
    } else {
        // Single IP
        if (ipRange.Contains(".")) {
            baseIP = ipRange.BeforeLast('.');
            startIP = wxAtoi(ipRange.AfterLast('.'));
            endIP = startIP;
        }
    }
    
    if (baseIP.IsEmpty()) {
        wxMessageBox("Invalid IP range format. Use format: 192.168.1.1-254", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    // Start scanning
    m_scanning = true;
    m_startScanButton->Enable(false);
    m_stopScanButton->Enable(true);
    m_hostsList->Clear();
    
    // Create network scanner and start scan
    m_networkScanner = std::make_unique<MasoudyCore::NetworkScanner>();
    if (m_networkScanner->Initialize()) {
        if (m_networkScanner->StartScan(baseIP.ToStdString(), startIP, endIP, 1000)) {
            // Start update timer
            m_updateTimer = std::make_unique<wxTimer>(this, ID_UPDATE_TIMER);
            m_updateTimer->Start(100); // Update every 100ms
        } else {
            wxMessageBox("Failed to start network scan", "Error", wxOK | wxICON_ERROR);
            m_scanning = false;
            m_startScanButton->Enable(true);
            m_stopScanButton->Enable(false);
        }
    } else {
        wxMessageBox("Failed to initialize network scanner", "Error", wxOK | wxICON_ERROR);
        m_scanning = false;
        m_startScanButton->Enable(true);
        m_stopScanButton->Enable(false);
    }
}

void NetworkScannerDialog::OnStopScan(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    
    if (!m_scanning) {
        return;
    }
    
    // Stop scanning
    if (m_networkScanner) {
        m_networkScanner->StopScan();
    }
    
    if (m_updateTimer) {
        m_updateTimer->Stop();
    }
    
    m_scanning = false;
    m_startScanButton->Enable(true);
    m_stopScanButton->Enable(false);
    
    // Update final results
    UpdateHostsList();
}

void NetworkScannerDialog::OnConnect(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    
    int selection = m_hostsList->GetSelection();
    if (selection == wxNOT_FOUND) {
        wxMessageBox("Please select a host to connect to", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    wxString selectedHost = m_hostsList->GetString(selection);
    if (selectedHost.IsEmpty()) {
        return;
    }
    
    // Extract IP from the list item
    wxString ip = selectedHost.BeforeFirst(' ');
    if (ip.IsEmpty()) {
        return;
    }
    
    // Store selected host for return
    m_selectedHost = ip.ToStdString();
    
    // Close dialog with OK
    EndModal(wxID_OK);
}

void NetworkScannerDialog::OnManualEntry(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    
    wxString ip = wxGetTextFromUser("Enter target IP address:", "Manual Entry", "192.168.1.100");
    if (!ip.IsEmpty()) {
        m_selectedHost = ip.ToStdString();
        EndModal(wxID_OK);
    }
}

void NetworkScannerDialog::OnCancel(wxCommandEvent& event) {
    (void)event; // Suppress unused parameter warning
    
    // Stop scanning if active
    if (m_scanning) {
        OnStopScan(event);
    }
    
    EndModal(wxID_CANCEL);
}

std::string NetworkScannerDialog::GetSelectedHost() const {
    return m_selectedHost;
} 

void NetworkScannerDialog::OnUpdateTimer(wxTimerEvent& event) {
    (void)event; // Suppress unused parameter warning
    
    // Update the hosts list with new discoveries
    UpdateHostsList();
    
    // Check if scanning is complete
    if (m_networkScanner && !m_networkScanner->IsScanning()) {
        m_updateTimer->Stop();
        m_scanning = false;
        m_startScanButton->Enable(true);
        m_stopScanButton->Enable(false);
    }
} 