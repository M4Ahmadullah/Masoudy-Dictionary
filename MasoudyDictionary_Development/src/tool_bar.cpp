#include "tool_bar.h"
#include "masoudy_core.h"
#include <wx/wx.h>
#include <wx/toolbar.h>
#include <wx/bitmap.h>
#include <wx/button.h>

// Tool IDs
enum {
    ID_TOOL_STEALTH_MODE = 6000,
    ID_TOOL_EXAM_MODE,
    ID_TOOL_NETWORK_SCANNER,
    ID_TOOL_SCREEN_SHARING,
    ID_TOOL_REMOTE_CONTROL,
    ID_TOOL_SETTINGS
};

ToolBar::ToolBar(wxWindow* parent)
    : wxToolBar(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
                wxTB_FLAT | wxTB_HORIZONTAL | wxTB_TEXT) {
    
    CreateTools();
}

ToolBar::~ToolBar() {
}

void ToolBar::UpdateState(MasoudyCore::AppMode mode, bool stealthMode) {
    UpdateModeTools(mode);
    UpdateConnectionTools(false); // Will be implemented when core integration is complete
}

void ToolBar::CreateTools() {
    // Add tools to toolbar
    AddTool(ID_TOOL_STEALTH_MODE, "Stealth Mode", wxNullBitmap, "Enable Stealth Mode");
    AddTool(ID_TOOL_EXAM_MODE, "Exam Mode", wxNullBitmap, "Enable Exam Mode");
    AddSeparator();
    AddTool(ID_TOOL_NETWORK_SCANNER, "Network Scanner", wxNullBitmap, "Open Network Scanner");
    AddTool(ID_TOOL_SCREEN_SHARING, "Screen Sharing", wxNullBitmap, "Start Screen Sharing");
    AddTool(ID_TOOL_REMOTE_CONTROL, "Remote Control", wxNullBitmap, "Enable Remote Control");
    AddSeparator();
    AddTool(ID_TOOL_SETTINGS, "Settings", wxNullBitmap, "Open Settings");
    
    Realize();
}

void ToolBar::UpdateModeTools(MasoudyCore::AppMode mode) {
    // Update tool states based on current mode
    ToggleTool(ID_TOOL_STEALTH_MODE, mode == MasoudyCore::AppMode::STEALTH_MODE);
    ToggleTool(ID_TOOL_EXAM_MODE, mode == MasoudyCore::AppMode::RESEARCH_MODE);
}

void ToolBar::UpdateConnectionTools(bool connected) {
    // Update connection-dependent tools
    EnableTool(ID_TOOL_NETWORK_SCANNER, !connected);
    EnableTool(ID_TOOL_SCREEN_SHARING, connected);
    ToggleTool(ID_TOOL_REMOTE_CONTROL, connected);
} 