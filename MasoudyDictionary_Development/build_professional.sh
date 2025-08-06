#!/bin/bash

# ========================================
# Masoudy Dictionary - Professional Build System
# ========================================
# This script works like "npm run build" for C++ applications
# It automatically builds, packages, and creates installers

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_header() {
    echo -e "${CYAN}========================================"
    echo "Masoudy Dictionary - Professional Build"
    echo "========================================"
    echo -e "${NC}"
}

print_footer() {
    echo -e "${CYAN}========================================"
    echo "Build completed successfully!"
    echo "========================================"
    echo -e "${NC}"
}

# Function to check dependencies
check_dependencies() {
    print_status "Checking build dependencies..."
    
    # Check for CMake
    if ! command -v cmake &> /dev/null; then
        print_error "CMake not found! Please install CMake."
        exit 1
    fi
    
    # Check for MinGW (for cross-compilation)
    if ! command -v x86_64-w64-mingw32-g++ &> /dev/null; then
        print_warning "MinGW-w64 not found. Install with: brew install mingw-w64"
        print_warning "Cross-compilation will not be available."
    fi
    
    # Check for NSIS (optional)
    if ! command -v makensis &> /dev/null; then
        print_warning "NSIS not found. Installer creation will be limited."
    fi
    
    print_status "Dependencies check completed."
}

# Function to clean previous builds
clean_build() {
    print_status "Cleaning previous builds..."
    rm -rf build_professional
    rm -rf MasoudyDictionary_Production
    mkdir -p build_professional
    print_status "Clean completed."
}

# Function to configure build
configure_build() {
    print_status "Configuring build system..."
    
    cd build_professional
    
    # Check if we're cross-compiling for Windows
    if command -v x86_64-w64-mingw32-g++ &> /dev/null; then
        print_status "Configuring for Windows cross-compilation..."
        
        # Configure with MinGW cross-compilation
        cmake .. \
            -DCMAKE_SYSTEM_NAME=Windows \
            -DCMAKE_SYSTEM_PROCESSOR=x86_64 \
            -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
            -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
            -DCMAKE_RC_COMPILER=x86_64-w64-mingw32-windres \
            -DCMAKE_BUILD_TYPE=Release \
            -DCMAKE_CXX_FLAGS="-DWIN32 -D_WIN32 -D_WINDOWS -D_UNICODE -DUNICODE -DWIN32_LEAN_AND_MEAN -DNOMINMAX -D_WIN32_WINNT=0x0601 -static -static-libgcc -static-libstdc++ -O2 -s" \
            -DCMAKE_C_FLAGS="-DWIN32 -D_WIN32 -D_WINDOWS -D_UNICODE -DUNICODE -DWIN32_LEAN_AND_MEAN -DNOMINMAX -D_WIN32_WINNT=0x0601 -static -static-libgcc -static-libstdc++ -O2 -s"
    else
        print_status "Configuring for native build..."
        
        # Configure for native build (without Windows-specific flags)
        cmake .. \
            -DCMAKE_BUILD_TYPE=Release \
            -DCMAKE_CXX_FLAGS="-O2" \
            -DCMAKE_C_FLAGS="-O2"
    fi
    
    if [ $? -ne 0 ]; then
        print_error "CMake configuration failed!"
        exit 1
    fi
    
    print_status "Build system configured."
}

# Function to build application
build_application() {
    print_status "Building application..."
    
    # Build the application
    cmake --build . --config Release --parallel
    
    if [ $? -ne 0 ]; then
        print_error "Build failed!"
        exit 1
    fi
    
    print_status "Application built successfully."
}

# Function to create professional package
create_package() {
    print_status "Creating professional package..."
    
    # Create package using CPack
    cmake --build . --target package
    
    if [ $? -ne 0 ]; then
        print_warning "CPack failed, creating manual package..."
        create_manual_package
    else
        print_status "Professional package created."
    fi
}

# Function to create manual package (fallback)
create_manual_package() {
    print_status "Creating manual package structure..."
    
    # Create production directory
    mkdir -p ../MasoudyDictionary_Production
    
    # Copy executable
    if [ -f "bin/MasoudyDictionary.exe" ]; then
        cp bin/MasoudyDictionary.exe ../MasoudyDictionary_Production/
    elif [ -f "bin/Release/MasoudyDictionary.exe" ]; then
        cp bin/Release/MasoudyDictionary.exe ../MasoudyDictionary_Production/
    else
        print_error "Executable not found!"
        exit 1
    fi
    
    # Copy additional files
    cp -r ../config ../MasoudyDictionary_Production/ 2>/dev/null || mkdir -p ../MasoudyDictionary_Production/config
    cp -r ../data ../MasoudyDictionary_Production/ 2>/dev/null || mkdir -p ../MasoudyDictionary_Production/data
    cp -r ../lang ../MasoudyDictionary_Production/ 2>/dev/null || mkdir -p ../MasoudyDictionary_Production/lang
    cp -r ../plugins ../MasoudyDictionary_Production/ 2>/dev/null || mkdir -p ../MasoudyDictionary_Production/plugins
    cp -r ../themes ../MasoudyDictionary_Production/ 2>/dev/null || mkdir -p ../MasoudyDictionary_Production/themes
    cp -r ../resources ../MasoudyDictionary_Production/ 2>/dev/null || mkdir -p ../MasoudyDictionary_Production/resources
    cp -r ../docs ../MasoudyDictionary_Production/ 2>/dev/null || mkdir -p ../MasoudyDictionary_Production/docs
    
    # Copy additional files
    cp ../LICENSE ../MasoudyDictionary_Production/ 2>/dev/null || echo "# MIT License" > ../MasoudyDictionary_Production/LICENSE
    cp ../README.txt ../MasoudyDictionary_Production/ 2>/dev/null || echo "Masoudy Dictionary" > ../MasoudyDictionary_Production/README.txt
    cp ../version.txt ../MasoudyDictionary_Production/ 2>/dev/null || echo "v1.0.0" > ../MasoudyDictionary_Production/version.txt
    
    # Create logs directory
    mkdir -p ../MasoudyDictionary_Production/logs
    
    print_status "Manual package created."
}

# Function to create installer
create_installer() {
    print_status "Creating professional installer..."
    
    if command -v makensis &> /dev/null; then
        cd ..
        makensis installer.nsi
        if [ $? -eq 0 ]; then
            print_status "Professional installer created: MasoudyDictionary_Professional_Installer.exe"
        else
            print_warning "NSIS compilation failed."
        fi
    else
        print_warning "NSIS not available. Installer creation skipped."
    fi
}

# Function to show build summary
show_summary() {
    print_status "Build Summary:"
    echo
    
    # Show executable info
    if [ -f "MasoudyDictionary_Production/MasoudyDictionary.exe" ]; then
        EXE_SIZE=$(stat -f%z "MasoudyDictionary_Production/MasoudyDictionary.exe" 2>/dev/null || echo "Unknown")
        echo "✅ Application: MasoudyDictionary.exe"
        echo "   Size: $EXE_SIZE bytes"
        echo
    fi
    
    # Show installer info
    if [ -f "MasoudyDictionary_Professional_Installer.exe" ]; then
        INSTALLER_SIZE=$(stat -f%z "MasoudyDictionary_Professional_Installer.exe" 2>/dev/null || echo "Unknown")
        echo "✅ Installer: MasoudyDictionary_Professional_Installer.exe"
        echo "   Size: $INSTALLER_SIZE bytes"
        echo
    fi
    
    # Show package info
    if [ -d "MasoudyDictionary_Production" ]; then
        FILE_COUNT=$(find MasoudyDictionary_Production -type f | wc -l)
        echo "✅ Package: MasoudyDictionary_Production/"
        echo "   Files: $FILE_COUNT"
        echo
    fi
    
    echo "Next Steps:"
    echo "1. Test: Run MasoudyDictionary.exe on Windows"
    echo "2. Install: Use the professional installer"
    echo "3. Distribute: Share the complete package"
    echo
}

# Function to create development build
dev_build() {
    print_status "Creating development build..."
    
    cd build_professional
    cmake --build . --config Debug
    print_status "Development build completed."
}

# Function to create clean build
clean_build_full() {
    print_status "Creating clean build..."
    clean_build
    configure_build
    build_application
    create_package
    create_installer
    show_summary
}

# Main execution
main() {
    print_header
    
    # Check command line arguments
    case "${1:-build}" in
        "build")
            print_status "Starting professional build..."
            check_dependencies
            clean_build
            configure_build
            build_application
            create_package
            create_installer
            show_summary
            ;;
        "dev")
            print_status "Starting development build..."
            check_dependencies
            clean_build
            configure_build
            dev_build
            ;;
        "clean")
            print_status "Starting clean build..."
            check_dependencies
            clean_build_full
            ;;
        "package")
            print_status "Creating package only..."
            check_dependencies
            configure_build
            create_package
            create_installer
            show_summary
            ;;
        "help")
            echo "Usage: $0 [command]"
            echo ""
            echo "Commands:"
            echo "  build   - Full professional build (default)"
            echo "  dev     - Development build"
            echo "  clean   - Clean build with full rebuild"
            echo "  package - Package only (no build)"
            echo "  help    - Show this help"
            ;;
        *)
            print_error "Unknown command: $1"
            echo "Use '$0 help' for usage information."
            exit 1
            ;;
    esac
    
    print_footer
}

# Run main function
main "$@" 