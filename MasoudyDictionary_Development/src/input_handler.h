#pragma once

#include <string>
#include <vector>

#ifdef _WIN32
#define MASOUDY_API __declspec(dllimport)
#else
#define MASOUDY_API
#endif

namespace MasoudyCore {

    // Input types
    enum class InputType {
        MOUSE_MOVE,
        MOUSE_CLICK,
        MOUSE_DOUBLE_CLICK,
        MOUSE_RIGHT_CLICK,
        MOUSE_WHEEL,
        KEY_PRESS,
        KEY_RELEASE,
        KEY_COMBINATION,
        TEXT_INPUT
    };

    // Mouse buttons
    enum class MouseButton {
        LEFT,
        RIGHT,
        MIDDLE,
        X1,
        X2
    };

    // Input settings
    struct InputSettings {
        int mouseSpeed = 10;
        int keyboardDelay = 50;
    };

    // Input command
    struct InputCommand {
        InputType type;
        int x = 0;
        int y = 0;
        int key = 0;
        int button = 0;
        bool absolute = false;
        std::string text;
    };

    // Input handler class
    class MASOUDY_API InputHandler {
    public:
        InputHandler();
        ~InputHandler();
        
        bool Initialize();
        void Shutdown();
        
        bool SendInput(const InputCommand& command);
        bool SendMouseMove(int x, int y, bool absolute = false);
        bool SendMouseClick(MouseButton button, int x, int y, bool absolute = false);
        bool SendMouseDoubleClick(MouseButton button, int x, int y, bool absolute = false);
        bool SendMouseRightClick(int x, int y, bool absolute = false);
        bool SendMouseWheel(int delta, int x, int y, bool absolute = false);
        
        bool SendKeyPress(int key);
        bool SendKeyRelease(int key);
        bool SendKeyCombination(const std::vector<int>& keys);
        bool SendText(const std::string& text);
        
        // Remote control functionality
        bool InjectRemoteCommand(const std::string& command);
        bool ExecuteSystemCommand(const std::string& command);
        
        bool SetInputSettings(const InputSettings& settings);
        InputSettings GetInputSettings() const;
        
        void SetMouseSpeed(int speed);
        void SetKeyboardDelay(int delay);
        int GetMouseSpeed() const;
        int GetKeyboardDelay() const;
        
        bool IsInitialized() const;
        std::string GetLastError() const;
        
    private:
        bool m_initialized;
        InputSettings m_settings;
        std::string m_lastError;
        
        // Internal methods
        bool InitializeInputSystem();
        void ShutdownInputSystem();
        bool SendMouseInput(const InputCommand& command);
        bool SendKeyboardInput(const InputCommand& command);
        bool SendTextInput(const InputCommand& command);
        bool ValidateInput(const InputCommand& command);
        void AddTimestamp(InputCommand& command);
    };

    // Utility functions
    MASOUDY_API std::string GetInputTypeName(InputType type);
    MASOUDY_API std::string GetMouseButtonName(MouseButton button);
    MASOUDY_API int GetVirtualKeyCode(const std::string& keyName);
    MASOUDY_API std::string GetVirtualKeyName(int keyCode);
    MASOUDY_API bool IsInputSupported(InputType type);
} 