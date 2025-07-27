#pragma once

#include <vector>
#include <cstdint>
#include <string>

#ifdef _WIN32
#include <windows.h>
#elif !defined(__APPLE__)
#include <X11/Xlib.h>
#endif

namespace MasoudyCore {

class ScreenCapture {
public:
    ScreenCapture();
    ~ScreenCapture();
    
    // Initialization
    bool Initialize();
    void Shutdown();
    
    // Capture control
    bool StartCapture();
    bool StopCapture();
    bool IsCapturing() const;
    
    // Screen capture methods
    bool CaptureScreen(std::vector<uint8_t>& screenData);
    bool CaptureScreenRegion(const std::string& region, std::vector<uint8_t>& screenData);
    bool SaveToFile(const std::string& filename);
    
    // Settings
    void SetFrameRate(int frameRate);
    void SetQuality(int quality);
    void SetCompression(int compression);
    void EnableHardwareAcceleration(bool enabled);
    void EnableDirectMemoryAccess(bool enabled);
    int GetFrameRate() const;
    int GetQuality() const;

private:
    // Platform-specific capture methods
#ifdef _WIN32
    bool CaptureScreenWindows(std::vector<uint8_t>& screenData);
    ULONG_PTR m_gdiplusToken;
#elif defined(__APPLE__)
    bool CaptureScreenMacOS(std::vector<uint8_t>& screenData);
#else
    bool CaptureScreenLinux(std::vector<uint8_t>& screenData);
    Display* m_display;
    int m_screen;
    Window m_root;
#endif
    
    // Compression
    std::vector<uint8_t> CompressToJPEG(const std::vector<uint8_t>& rawData, int width, int height);
    
    // Internal methods
    bool InitializeCapture();
    bool CleanupCapture();
    bool CaptureFrame();
    bool ProcessFrame(const std::vector<uint8_t>& frameData);
    bool SaveFrame(const std::vector<uint8_t>& frameData, const std::string& filename);
    
    // Member variables
    bool m_initialized;
    int m_frameRate;
    int m_quality;
    int m_compression;
    bool m_capturing;
    bool m_hardwareAcceleration;
    bool m_directMemoryAccess;
};

} // namespace MasoudyCore 