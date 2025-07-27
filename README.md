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
- **Visual Studio 2022** (Community Edition is free)
- **wxWidgets 3.3** (static linking)
- **CMake 3.20+**
- **NSIS** for installer creation
- **Git** for cloning

### macOS (Development)
- Xcode Command Line Tools
- Homebrew: `brew install wxwidgets cmake`
- Clang/LLVM

## 🔧 Quick Start - Windows

### Step 1: Clone the Repository
```bash
# Open Command Prompt or PowerShell
git clone https://github.com/M4Ahmadullah/Masoudy-Dictionary.git
cd Masoudy-Dictionary
```

### Step 2: Install Dependencies

#### Option A: Using vcpkg (Recommended)
```bash
# Install vcpkg if you don't have it
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install

# Install wxWidgets
.\vcpkg install wxwidgets:x64-windows-static
```

#### Option B: Manual Installation
1. **Download wxWidgets 3.3** from https://www.wxwidgets.org/downloads/
2. **Extract to C:\wxWidgets-3.3**
3. **Build wxWidgets** (see wxWidgets documentation)

### Step 3: Build the Application
```bash
# Navigate to development folder
cd MasoudyDictionary_Development

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build . --config Release
```

### Step 4: Create Production Installer
```bash
# Run the production build script
cd ..
.\build_windows_production.bat
```

### Step 5: Install and Run
1. **Find the installer**: `MasoudyDictionary_Production/Versions/Masoudy-Dictionary-V-1.0/`
2. **Run installer**: `Masoudy-Dictionary-Setup.exe`
3. **Launch application**: Start from Start Menu or Desktop shortcut

## 🔧 Building from Source - Detailed Instructions

### Prerequisites Installation

#### 1. Install Visual Studio 2022
```bash
# Download from: https://visualstudio.microsoft.com/downloads/
# Install with C++ development workload
# Include: MSVC v143, Windows 10/11 SDK, CMake tools
```

#### 2. Install wxWidgets
```bash
# Method 1: Using vcpkg (Easiest)
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
.\vcpkg install wxwidgets:x64-windows-static

# Method 2: Manual build
# Download wxWidgets 3.3 source
# Build with: nmake -f makefile.vc BUILD=release SHARED=0
```

#### 3. Install CMake
```bash
# Download from: https://cmake.org/download/
# Add to PATH during installation
```

### Build Process

#### 1. Clone Repository
```bash
git clone https://github.com/M4Ahmadullah/Masoudy-Dictionary.git
cd Masoudy-Dictionary/MasoudyDictionary_Development
```

#### 2. Configure Build
```bash
mkdir build
cd build

# Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64 ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DwxWidgets_ROOT_DIR=C:/wxWidgets-3.3 ^
    -DwxWidgets_CONFIG_EXECUTABLE=C:/wxWidgets-3.3/build/msw/wx-config.exe
```

#### 3. Build Application
```bash
# Build in Release mode
cmake --build . --config Release --parallel

# Or open in Visual Studio
start MasoudyDictionary.sln
```

#### 4. Create Installer
```bash
# Install NSIS if not already installed
# Download from: https://nsis.sourceforge.io/Download

# Run installer creation
cd ..
.\build_windows_production.bat
```

## 🎯 Usage Guide

### Network Discovery
1. **Launch the application**
2. **Click "Network Scanner"** from Tools menu
3. **Enter IP range** (e.g., "192.168.1.1-254")
4. **Click "Start Scan"** to discover hosts
5. **Select target host** and click "Connect"

### Screen Sharing
1. **Establish connection** to target
2. **Click "Screen Sharing"** from Tools menu
3. **Configure quality** and frame rate settings
4. **Click "Start Sharing"** to begin capture
5. **Receiver displays** real-time screen

### Stealth Mode
1. **Click "Stealth Mode"** from Mode menu
2. **Application becomes invisible** to monitoring tools
3. **Process hidden** from Task Manager
4. **Network traffic obfuscated**

### Exam Mode
1. **Click "Exam Mode"** from Mode menu
2. **Application window disappears** completely
3. **Screen sharing continues** in background
4. **No visible indicators** of operation

### Remote Control
1. **Enable remote control** on target system
2. **Send mouse movements**: `MOUSE_MOVE x y`
3. **Send keyboard input**: `KEY_PRESS keycode`
4. **Execute system commands**: `SYSTEM_COMMAND "cmd"`

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

## 🚨 Troubleshooting

### Common Build Issues

#### 1. wxWidgets Not Found
```bash
# Solution: Set wxWidgets path explicitly
cmake .. -DwxWidgets_ROOT_DIR=C:/wxWidgets-3.3
```

#### 2. CMake Configuration Errors
```bash
# Solution: Install Visual Studio 2022 with C++ workload
# Ensure Windows SDK is installed
```

#### 3. Linker Errors
```bash
# Solution: Use static linking
cmake .. -DwxWidgets_USE_STATIC=ON
```

#### 4. Runtime Errors
```bash
# Solution: Install Visual C++ Redistributable
# Download from Microsoft website
```

### Performance Optimization
```bash
# Build with optimizations
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE="/O2 /MT"

# Enable parallel builds
cmake --build . --parallel
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