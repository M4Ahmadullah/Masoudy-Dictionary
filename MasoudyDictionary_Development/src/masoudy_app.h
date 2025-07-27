#pragma once
#include <wx/wx.h>

class MasoudyApp : public wxApp {
public:
    MasoudyApp();
    virtual ~MasoudyApp();

    // wxApp overrides
    virtual bool OnInit();
    virtual int OnRun();
    virtual int OnExit();

    // Application settings
    void SetStealthMode(bool enabled);
    void SetExamMode(bool enabled);

private:
    bool m_stealthMode;
    bool m_examMode;
}; 