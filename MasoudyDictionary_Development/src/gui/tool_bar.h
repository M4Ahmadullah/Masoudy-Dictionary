#ifndef TOOL_BAR_H
#define TOOL_BAR_H

#include <wx/wx.h>
#include <wx/toolbar.h>
#include <wx/bitmap.h>
#include <wx/button.h>

// Forward declarations
namespace MasoudyCore {
    enum class AppMode;
}

class ToolBar : public wxToolBar {
public:
    ToolBar(wxWindow* parent);
    virtual ~ToolBar();

    // Toolbar operations
    void UpdateState(MasoudyCore::AppMode mode, bool stealthMode);

private:
    // Toolbar creation
    void CreateTools();
    void UpdateModeTools(MasoudyCore::AppMode mode);
    void UpdateConnectionTools(bool connected);
};

#endif // TOOL_BAR_H 