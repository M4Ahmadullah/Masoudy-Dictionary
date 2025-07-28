#include "menu_bar.h"
#include "masoudy_core.h"
#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/menuitem.h>

// Menu item IDs
enum {
    ID_NEW_SESSION = 5000,
    ID_OPEN_SESSION,
    ID_SAVE_SESSION,
    ID_FULL_SCREEN,
    ID_ALWAYS_ON_TOP,
    ID_EXAM_MODE,
    ID_STEALTH_MODE,
    ID_NETWORK_SCANNER,
    ID_SCREEN_SHARING,
    ID_REMOTE_CONTROL,
    ID_HELP_MENU
};

MenuBar::MenuBar() {
    CreateFileMenu();
    CreateEditMenu();
    CreateViewMenu();
    CreateModeMenu();
    CreateToolsMenu();
    CreateHelpMenu();
}

MenuBar::~MenuBar() {
}

void MenuBar::UpdateState(MasoudyCore::AppMode mode, bool stealthMode, bool fullScreen, bool alwaysOnTop) {
    UpdateModeMenu(mode);
    UpdateViewMenu(fullScreen, alwaysOnTop);
    UpdateToolsMenu(false); // Connection state will be implemented when core integration is complete
}

void MenuBar::CreateFileMenu() {
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(ID_NEW_SESSION, "&New\tCtrl+N", "Create new session");
    fileMenu->Append(ID_OPEN_SESSION, "&Open\tCtrl+O", "Open session");
    fileMenu->Append(ID_SAVE_SESSION, "&Save\tCtrl+S", "Save session");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "E&xit\tCtrl+Q", "Exit the application");
    Append(fileMenu, "&File");
}

void MenuBar::CreateEditMenu() {
    wxMenu* editMenu = new wxMenu();
    editMenu->Append(wxID_UNDO, "&Undo\tCtrl+Z", "Undo last action");
    editMenu->Append(wxID_REDO, "&Redo\tCtrl+Y", "Redo last action");
    editMenu->AppendSeparator();
    editMenu->Append(wxID_COPY, "&Copy\tCtrl+C", "Copy selection");
    editMenu->Append(wxID_PASTE, "&Paste\tCtrl+V", "Paste selection");
    Append(editMenu, "&Edit");
}

void MenuBar::CreateViewMenu() {
    wxMenu* viewMenu = new wxMenu();
    m_fullScreenItem = viewMenu->AppendCheckItem(ID_FULL_SCREEN, "&Full Screen\tF11", "Toggle full screen");
    m_alwaysOnTopItem = viewMenu->AppendCheckItem(ID_ALWAYS_ON_TOP, "&Always on Top", "Keep window on top");
    Append(viewMenu, "&View");
}

void MenuBar::CreateModeMenu() {
    wxMenu* modeMenu = new wxMenu();
    m_examModeItem = modeMenu->AppendCheckItem(ID_EXAM_MODE, "&Exam Mode\tCtrl+E", "Enable exam mode");
    m_stealthModeItem = modeMenu->AppendCheckItem(ID_STEALTH_MODE, "&Stealth Mode\tCtrl+H", "Enable stealth mode");
    Append(modeMenu, "&Mode");
}

void MenuBar::CreateToolsMenu() {
    wxMenu* toolsMenu = new wxMenu();
    m_networkScannerItem = toolsMenu->Append(ID_NETWORK_SCANNER, "&Network Scanner", "Open network scanner");
    m_screenSharingItem = toolsMenu->Append(ID_SCREEN_SHARING, "&Screen Sharing", "Start screen sharing");
    toolsMenu->AppendSeparator();
    m_remoteControlItem = toolsMenu->AppendCheckItem(ID_REMOTE_CONTROL, "&Remote Control", "Enable remote control");
    Append(toolsMenu, "&Tools");
}

void MenuBar::CreateHelpMenu() {
    wxMenu* helpMenu = new wxMenu();
    helpMenu->Append(wxID_ABOUT, "&About\tF1", "About this application");
    helpMenu->Append(ID_HELP_MENU, "&Help\tF1", "Show help");
    Append(helpMenu, "&Help");
}

void MenuBar::UpdateModeMenu(MasoudyCore::AppMode mode) {
    if (m_examModeItem) {
        m_examModeItem->Check(mode == MasoudyCore::AppMode::RESEARCH_MODE);
    }
    if (m_stealthModeItem) {
        m_stealthModeItem->Check(mode == MasoudyCore::AppMode::STEALTH_MODE);
    }
}

void MenuBar::UpdateViewMenu(bool fullScreen, bool alwaysOnTop) {
    if (m_fullScreenItem) {
        m_fullScreenItem->Check(fullScreen);
    }
    if (m_alwaysOnTopItem) {
        m_alwaysOnTopItem->Check(alwaysOnTop);
    }
}

void MenuBar::UpdateToolsMenu(bool connected) {
    if (m_networkScannerItem) {
        m_networkScannerItem->Enable(!connected); // Disable when connected
    }
    if (m_screenSharingItem) {
        m_screenSharingItem->Enable(connected); // Enable when connected
    }
    if (m_remoteControlItem) {
        m_remoteControlItem->Check(connected);
    }
} 