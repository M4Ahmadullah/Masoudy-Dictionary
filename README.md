# Masoudy Dictionary

## Advanced Exam-Proof Cybersecurity Application

A professional-grade cybersecurity application designed for advanced network monitoring and remote control capabilities with complete stealth and anti-detection features.

## 🚀 Features

### Core Functionality
- **Real-time Screen Sharing**: Windows GDI+ screen capture with compression
- **Network Discovery**: Automatic LAN scanning and host detection
- **Secure Communication**: Encrypted TCP/IP communication
- **Remote Control**: Full mouse, keyboard, and system command injection
- **Complete Stealth**: 100% invisible to security monitoring tools

### Anti-Detection Capabilities
- **Process Invisibility**: PEB unlinking and thread hiding
- **Memory Encryption**: Code obfuscation and memory protection
- **Network Obfuscation**: Traffic disguised as legitimate HTTPS
- **Anti-Debugging**: Timing attacks and direct syscalls
- **Anti-VM/Sandbox**: Detection and evasion of analysis environments

### Professional GUI
- **Modern Interface**: Clean wxWidgets-based GUI
- **Network Scanner**: Real-time host discovery dialog
- **Screen Sharing Panel**: Quality and frame rate controls
- **Stealth Panel**: Anti-detection configuration
- **Settings Management**: Comprehensive configuration options

## 🛠️ Technology Stack

- **Language**: C++17
- **GUI Framework**: wxWidgets 3.3
- **Build System**: CMake
- **Platform**: Windows (Production), macOS (Development)
- **Linking**: Static linking for self-contained deployment
- **Packaging**: NSIS installer for Windows

## 📋 Requirements

### Windows (Production)
- Visual Studio 2022
- wxWidgets 3.3 (static linking)
- CMake 3.20+
- NSIS for installer

### macOS (Development)
- Xcode Command Line Tools
- Homebrew: `brew install wxwidgets cmake`
- Clang/LLVM

## 🔧 Building

### Windows Production Build
```bash
# Clone repository
git clone https://github.com/M4Ahmadullah/Masoudy-Dictionary.git
cd Masoudy-Dictionary

# Build with CMake
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release

# Create installer
./build_windows_production.bat
```

### macOS Development Build
```bash
# Install dependencies
brew install wxwidgets cmake

# Build for validation
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

## 🎯 Usage

### Network Discovery
1. Launch the application
2. Click "Network Scanner" from Tools menu
3. Enter IP range (e.g., "192.168.1.1-254")
4. Click "Start Scan" to discover hosts
5. Select target host and click "Connect"

### Screen Sharing
1. Establish connection to target
2. Click "Screen Sharing" from Tools menu
3. Configure quality and frame rate settings
4. Click "Start Sharing" to begin capture
5. Receiver will display real-time screen

### Stealth Mode
1. Click "Stealth Mode" from Mode menu
2. Application becomes invisible to monitoring tools
3. Process hidden from Task Manager
4. Network traffic obfuscated

### Exam Mode
1. Click "Exam Mode" from Mode menu
2. Application window disappears completely
3. Screen sharing continues in background
4. No visible indicators of operation

### Remote Control
1. Enable remote control on target system
2. Send mouse movements: `MOUSE_MOVE x y`
3. Send keyboard input: `KEY_PRESS keycode`
4. Execute system commands: `SYSTEM_COMMAND "cmd"`

## 🔒 Security Features

### Anti-Detection
- **Process Hiding**: Complete invisibility to monitoring tools
- **Memory Protection**: Encrypted code segments
- **Network Stealth**: Disguised traffic patterns
- **Behavioral Mimicry**: Legitimate process behavior simulation

### Communication Security
- **Encryption**: XOR-based message encryption
- **Compression**: Data compression for efficiency
- **Heartbeat**: Connection monitoring
- **Authentication**: Secure credential exchange

## 📁 Project Structure

```
MasoudyDictionary_Development/
├── src/                    # Source code
│   ├── main.cpp           # Application entry point
│   ├── masoudy_app.cpp    # Main application class
│   ├── main_frame.cpp     # Main window implementation
│   ├── network_scanner.cpp # Network discovery
│   ├── screen_capture.cpp # Screen capture
│   ├── stealth_engine.cpp # Anti-detection
│   ├── communication_engine.cpp # Network communication
│   ├── input_handler.cpp  # Remote control
│   └── ...               # Additional modules
├── docs/                  # Documentation
├── resources/             # Application resources
├── CMakeLists.txt        # Build configuration
└── build_windows_production.bat # Windows build script
```

## 🚨 Disclaimer

This application is designed for educational and authorized security testing purposes only. Users are responsible for ensuring compliance with applicable laws and regulations. The developers are not liable for misuse of this software.

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## 📞 Support

For questions or support, please open an issue on GitHub.

---

**Version**: 1.0.0  
**Last Updated**: 2024  
**Platform**: Windows (Production), macOS (Development) 