#include "masoudy_app.h"
#include "main_frame.h"
#include <wx/wx.h>

MasoudyApp::MasoudyApp()
    : m_stealthMode(false)
    , m_examMode(false) {
}

MasoudyApp::~MasoudyApp() {
}

bool MasoudyApp::OnInit() {
    // Create the main application window
    MainFrame* frame = new MainFrame(wxT("Masoudy Dictionary - Advanced Cybersecurity Application"));
    frame->Show();
    
    // Set as top window
    SetTopWindow(frame);
    
    return true;
}

int MasoudyApp::OnRun() {
    // Run the wxWidgets event loop
    return wxApp::OnRun();
}

int MasoudyApp::OnExit() {
    // Cleanup when application exits
    return wxApp::OnExit();
}

void MasoudyApp::SetStealthMode(bool enabled) {
    m_stealthMode = enabled;
}

void MasoudyApp::SetExamMode(bool enabled) {
    m_examMode = enabled;
} 