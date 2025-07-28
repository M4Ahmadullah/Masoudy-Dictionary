#include "login_dialog.h"
#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <iostream>
#include <string>

// Event IDs
enum {
    ID_LOGIN_BUTTON = 1000,
    ID_CANCEL_BUTTON = 1001,
    ID_ROLE_CHOICE = 1002
};

// Auth system stub
namespace MasoudyAuth {
    enum class AuthStatus {
        SUCCESS,
        INVALID_CREDENTIALS,
        ACCOUNT_LOCKED,
        SESSION_EXPIRED,
        ERROR
    };

    enum class UserRole {
        VICTIM,
        ADMIN
    };

    class AuthSystem {
    public:
        AuthStatus authenticate(const std::string& username, const std::string& password) {
            if (username == "victim" && password == "victim") {
                return AuthStatus::SUCCESS;
            } else if (username == "admin" && password == "admin") {
                return AuthStatus::SUCCESS;
            }
            return AuthStatus::INVALID_CREDENTIALS;
        }
        
        bool isAuthenticated() const { return true; }
        bool isAdmin() const { return true; }
    };
}

LoginDialog::LoginDialog(wxWindow* parent, MasoudyAuth::AuthSystem* authSystem)
    : wxDialog(parent, wxID_ANY, wxT("Masoudy Dictionary - Authentication"))
    , m_authSystem(authSystem) {
    
    std::cout << "LoginDialog constructor called" << std::endl;
    
    // Create main sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Title
    wxStaticText* titleLabel = new wxStaticText(this, wxID_ANY, 
        wxT("Masoudy Dictionary\nAdvanced Exam-Proof Cybersecurity Application"));
    mainSizer->Add(titleLabel, 0, wxALL | wxALIGN_CENTER, 10);
    
    // Authentication box
    wxStaticBox* authBox = new wxStaticBox(this, wxID_ANY, wxT("Authentication"));
    wxStaticBoxSizer* authSizer = new wxStaticBoxSizer(authBox, wxVERTICAL);
    
    // Role selection
    wxStaticText* roleLabel = new wxStaticText(this, wxID_ANY, wxT("Select Role:"));
    authSizer->Add(roleLabel, 0, wxALL, 5);
    
    m_roleChoice = new wxChoice(this, ID_ROLE_CHOICE);
    m_roleChoice->Append(wxT("Victim (Laptop 1 - Being Monitored)"));
    m_roleChoice->Append(wxT("Admin (Laptop 2 - Monitoring)"));
    m_roleChoice->SetSelection(0);
    authSizer->Add(m_roleChoice, 0, wxEXPAND | wxALL, 5);
    
    // Username
    wxStaticText* userLabel = new wxStaticText(this, wxID_ANY, wxT("Username:"));
    authSizer->Add(userLabel, 0, wxALL, 5);
    
    m_usernameCtrl = new wxTextCtrl(this, wxID_ANY, wxT("victim"));
    authSizer->Add(m_usernameCtrl, 0, wxEXPAND | wxALL, 5);
    
    // Password
    wxStaticText* passLabel = new wxStaticText(this, wxID_ANY, wxT("Password:"));
    authSizer->Add(passLabel, 0, wxALL, 5);
    
    m_passwordCtrl = new wxTextCtrl(this, wxID_ANY, wxT("victim"), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    authSizer->Add(m_passwordCtrl, 0, wxEXPAND | wxALL, 5);
    
    // Buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    
    m_loginButton = new wxButton(this, ID_LOGIN_BUTTON, wxT("Login"));
    m_cancelButton = new wxButton(this, ID_CANCEL_BUTTON, wxT("Cancel"));
    
    buttonSizer->Add(m_loginButton, 0, wxALL, 5);
    buttonSizer->Add(m_cancelButton, 0, wxALL, 5);
    
    authSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 10);
    
    // Status label
    m_statusLabel = new wxStaticText(this, wxID_ANY, wxT(""));
    authSizer->Add(m_statusLabel, 0, wxALIGN_CENTER | wxALL, 5);
    
    mainSizer->Add(authSizer, 1, wxEXPAND | wxALL, 10);
    
    SetSizer(mainSizer);
    Fit();
    Centre();
}

LoginDialog::~LoginDialog() {
    std::cout << "LoginDialog destructor called" << std::endl;
}

int LoginDialog::ShowModal() {
    return wxDialog::ShowModal();
}

wxString LoginDialog::GetUsername() const {
    return m_usernameCtrl->GetValue();
}

wxString LoginDialog::GetPassword() const {
    return m_passwordCtrl->GetValue();
}

wxString LoginDialog::GetRole() const {
    return m_roleChoice->GetString(m_roleChoice->GetSelection());
}

bool LoginDialog::IsAuthenticated() const {
    return m_authSystem && m_authSystem->isAuthenticated();
} 