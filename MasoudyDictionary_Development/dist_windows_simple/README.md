# Masoudy Dictionary

## Overview

Masoudy Dictionary is an advanced dictionary application with integrated screen sharing capabilities. It provides a comprehensive dictionary interface while offering powerful screen sharing features for educational and research purposes.

## Features

### Dictionary Features
- **Multi-language Support**: English, Arabic, Persian, and more
- **Offline Mode**: Works without internet connection
- **Custom Dictionaries**: Add your own word definitions
- **Search Suggestions**: Intelligent word suggestions
- **Pronunciation**: Audio pronunciation for words
- **Bookmarks**: Save frequently used words
- **Export/Import**: Share dictionaries with others

### Screen Sharing Features
- **Real-time Screen Capture**: View remote screens in real-time
- **Network Discovery**: Automatically find computers on the same network
- **Secure Communication**: Encrypted data transmission
- **Low Resource Usage**: Optimized for minimal system impact
- **Cross-platform Support**: Works on Windows, macOS, and Linux

### Advanced Features
- **Stealth Mode**: Hidden operation capabilities
- **Customizable Interface**: Multiple themes and layouts
- **Hotkey Support**: Quick access to common functions
- **System Tray Integration**: Minimize to system tray
- **Auto-startup**: Configure to start with Windows
- **Portable Mode**: Run from USB drive without installation

## System Requirements

### Minimum Requirements
- **OS**: Windows 7 SP1 or later
- **CPU**: 1 GHz processor
- **RAM**: 2 GB
- **Storage**: 100 MB free space
- **Network**: Ethernet or WiFi connection

### Recommended Requirements
- **OS**: Windows 10 or later
- **CPU**: 2 GHz dual-core processor
- **RAM**: 4 GB or more
- **Storage**: 500 MB free space
- **Network**: High-speed internet connection

## Installation

### Method 1: Windows Installer
1. Download the latest installer from the releases page
2. Run `MasoudyDictionary-Setup.exe`
3. Follow the installation wizard
4. Launch the application from the Start Menu or Desktop shortcut

### Method 2: Portable Version
1. Download the portable package
2. Extract to any folder (including USB drive)
3. Run `MasoudyDictionary.exe` directly

### Method 3: Build from Source
```bash
# Clone the repository
git clone https://github.com/masoudy/dictionary.git
cd dictionary

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build . --config Release

# Install (optional)
cmake --install .
```

## Usage

### Dictionary Mode
1. Launch the application
2. Select "Dictionary" mode from the main menu
3. Type a word in the search box
4. View definitions, translations, and examples
5. Use bookmarks to save important words

### Screen Sharing Mode
1. Launch the application on both computers
2. Select "Screen Sharing" mode
3. On the sender computer:
   - Click "Scan Network" to find available computers
   - Select the target computer from the list
   - Click "Start Sharing"
4. On the receiver computer:
   - Wait for incoming connection
   - View the shared screen in real-time

### Stealth Mode
1. Launch the application with `--stealth` flag
2. The application runs in the background
3. Use hotkeys to control screen sharing
4. No visible interface or system tray icon

## Configuration

### Application Settings
- **Language**: Set interface language
- **Dictionary Path**: Specify custom dictionary location
- **Auto-startup**: Enable/disable startup with Windows
- **System Tray**: Show/hide system tray icon
- **Hotkeys**: Customize keyboard shortcuts

### Screen Sharing Settings
- **Quality**: Adjust JPEG compression (1-100)
- **Frame Rate**: Set capture frequency (1-30 fps)
- **Encryption**: Enable/disable data encryption
- **Compression**: Enable/disable data compression
- **Network Timeout**: Set connection timeout

### Stealth Settings
- **Process Name**: Customize process name
- **Service Name**: Customize service name
- **Registry Keys**: Customize registry entries
- **Memory Protection**: Enable memory encryption
- **Network Obfuscation**: Enable traffic obfuscation

## Command Line Options

```bash
MasoudyDictionary.exe [options]

Options:
  -h, --help              Show help message
  -v, --version           Show version information
  -s, --stealth           Start in stealth mode
  -d, --dictionary        Start in dictionary mode only
  -t, --target <ip>       Target IP address for screen sharing
  -p, --port <number>     Port number for communication
  <dictionary_file>       Path to custom dictionary file
```

## Security Features

### Stealth Capabilities
- **Process Hiding**: Runs inside legitimate processes
- **Memory Protection**: Encrypts sensitive data in memory
- **Network Obfuscation**: Disguises network traffic
- **Anti-Detection**: Evades security tool detection
- **Behavioral Mimicry**: Mimics legitimate application behavior

### Communication Security
- **End-to-End Encryption**: All data is encrypted
- **Certificate Validation**: Verifies connection authenticity
- **Session Management**: Secure session handling
- **Access Control**: User authentication and authorization

## Troubleshooting

### Common Issues

**Application won't start**
- Check system requirements
- Run as administrator if needed
- Verify antivirus isn't blocking the application

**Screen sharing not working**
- Ensure both computers are on the same network
- Check firewall settings
- Verify network permissions

**Dictionary not loading**
- Check dictionary file path
- Verify file permissions
- Try reinstalling the application

**Performance issues**
- Reduce screen sharing quality
- Lower frame rate settings
- Close unnecessary applications

### Log Files
Log files are stored in:
- `%APPDATA%\Masoudy Dictionary\logs\`
- `%TEMP%\Masoudy Dictionary\logs\`

### Support
For technical support:
- Email: support@masoudy.com
- GitHub Issues: https://github.com/masoudy/dictionary/issues
- Documentation: https://docs.masoudy.com

## Development

### Building from Source
```bash
# Prerequisites
- CMake 3.10 or higher
- C++17 compatible compiler
- wxWidgets development libraries
- Windows SDK (for Windows builds)

# Build steps
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

### Project Structure
```
MasoudyDictionary/
├── masoudy_core/          # Core library
├── masoudy_gui/           # GUI application
├── masoudy_installer/     # Installer components
├── resources/             # Application resources
├── docs/                 # Documentation
└── tests/                # Unit tests
```

### Contributing
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- wxWidgets team for the GUI framework
- OpenSSL team for encryption libraries
- Contributors and beta testers

## Version History

### v1.0.0 (2024-01-01)
- Initial release
- Dictionary functionality
- Screen sharing capabilities
- Stealth mode
- Windows installer
- Portable version

## Roadmap

### v1.1.0 (Planned)
- Multi-monitor support
- Session recording
- Enhanced encryption
- Mobile companion app
- Cloud synchronization

### v1.2.0 (Planned)
- AI-powered translations
- Voice recognition
- Advanced stealth features
- Cross-platform installer
- Plugin system

---

**Note**: This application is designed for educational and research purposes. Users are responsible for ensuring compliance with local laws and regulations.
