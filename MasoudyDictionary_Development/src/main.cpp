#include <wx/wx.h>
#include "masoudy_app.h"

// Windows headers for WinMain
#ifdef _WIN32
#include <windows.h>
#endif

// Application entry point for wxWidgets
IMPLEMENT_APP(MasoudyApp)

// Windows entry point
#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, int nCmdShow) {
    return wxEntry(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
#endif 