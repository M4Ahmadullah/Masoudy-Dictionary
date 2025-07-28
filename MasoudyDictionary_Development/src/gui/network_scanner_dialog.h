#ifndef NETWORK_SCANNER_DIALOG_H
#define NETWORK_SCANNER_DIALOG_H

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/spinctrl.h>
#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/timer.h>
#include <vector>
#include <string>
#include <memory>

// Forward declarations
namespace MasoudyCore {
    struct NetworkHost;
    class NetworkScanner;
}

class NetworkScannerDialog : public wxDialog {
public:
    NetworkScannerDialog(wxWindow* parent);
    virtual ~NetworkScannerDialog();

    int ShowModal();
    std::string GetBaseIP() const;
    int GetStartIP() const;
    int GetEndIP() const;
    int GetTimeout() const;
    std::vector<MasoudyCore::NetworkHost> GetDiscoveredHosts() const;
    void SetDiscoveredHosts(const std::vector<MasoudyCore::NetworkHost>& hosts);
    std::string GetSelectedHost() const;

private:
    void CreateControls();
    void UpdateHostsList();
    void OnStartScan(wxCommandEvent& event);
    void OnStopScan(wxCommandEvent& event);
    void OnConnect(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    void OnManualEntry(wxCommandEvent& event);
    void OnUpdateTimer(wxTimerEvent& event);

    // UI components
    wxTextCtrl* m_ipRangeCtrl;
    wxTextCtrl* m_portRangeCtrl;
    wxListBox* m_hostsList;
    wxButton* m_startScanButton;
    wxButton* m_stopScanButton;
    wxButton* m_connectButton;
    wxButton* m_manualEntryButton;
    wxButton* m_cancelButton;

    // Data
    std::vector<MasoudyCore::NetworkHost> m_discoveredHosts;
    bool m_scanning;
    std::string m_selectedHost;
    
    // Network scanner components
    std::unique_ptr<MasoudyCore::NetworkScanner> m_networkScanner;
    std::unique_ptr<wxTimer> m_updateTimer;

    DECLARE_EVENT_TABLE()
};

#endif // NETWORK_SCANNER_DIALOG_H 