#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/sizer.h>

// Forward declaration
namespace MasoudyAuth {
    class AuthSystem;
    enum class AuthStatus;
    enum class UserRole;
}

class LoginDialog : public wxDialog {
public:
    LoginDialog(wxWindow* parent, MasoudyAuth::AuthSystem* authSystem);
    virtual ~LoginDialog();

    int ShowModal();
    wxString GetUsername() const;
    wxString GetPassword() const;
    wxString GetRole() const;
    
    // Additional methods
    bool IsAuthenticated() const;

private:
    void CreateControls();
    void OnLogin(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    void OnRoleChanged(wxCommandEvent& event);
    void OnTextChanged(wxCommandEvent& event);
    
    // Additional private methods
    void UpdateUI();
    void ShowError(const wxString& message);
    void ShowSuccess(const wxString& message);
    void ClearStatus();

    wxTextCtrl* m_usernameCtrl;
    wxTextCtrl* m_passwordCtrl;
    wxChoice* m_roleChoice;
    wxButton* m_loginButton;
    wxButton* m_cancelButton;
    wxStaticText* m_statusLabel;
    MasoudyAuth::AuthSystem* m_authSystem;

    wxString m_username;
    wxString m_password;
    wxString m_role;

    DECLARE_EVENT_TABLE()
};

#endif // LOGIN_DIALOG_H 