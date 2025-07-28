#ifndef MENU_BAR_H
#define MENU_BAR_H

#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/menuitem.h>

// Forward declarations
namespace MasoudyCore {
    enum class AppMode;
}

class MenuBar : public wxMenuBar {
public:
    MenuBar();
    virtual ~MenuBar();

    // Menu bar operations
    void UpdateState(MasoudyCore::AppMode mode, bool stealthMode, bool fullScreen, bool alwaysOnTop);

private:
    // Menu creation
    void CreateFileMenu();
    void CreateEditMenu();
    void CreateViewMenu();
    void CreateModeMenu();
    void CreateToolsMenu();
    void CreateHelpMenu();

    // Menu updates
    void UpdateModeMenu(MasoudyCore::AppMode mode);
    void UpdateViewMenu(bool fullScreen, bool alwaysOnTop);
    void UpdateToolsMenu(bool connected);

    // Menu item references for dynamic updates
    wxMenuItem* m_fullScreenItem;
    wxMenuItem* m_alwaysOnTopItem;
    wxMenuItem* m_examModeItem;
    wxMenuItem* m_stealthModeItem;
    wxMenuItem* m_networkScannerItem;
    wxMenuItem* m_screenSharingItem;
    wxMenuItem* m_remoteControlItem;
};

#endif // MENU_BAR_H 