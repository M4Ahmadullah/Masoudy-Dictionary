#!/bin/bash

# Cross-compilation build script for Windows from macOS
# This script builds a Windows executable directly from macOS

set -e  # Exit on any error

echo "🚀 Starting cross-compilation build for Windows from macOS..."

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if we're on macOS
if [[ "$OSTYPE" != "darwin"* ]]; then
    print_error "This script is designed for macOS only"
    exit 1
fi

# Check if MinGW is installed
if ! command -v x86_64-w64-mingw32-gcc &> /dev/null; then
    print_error "MinGW-w64 not found. Installing..."
    brew install mingw-w64
fi

# Check if wxWidgets is installed
if ! brew list wxwidgets &> /dev/null; then
    print_error "wxWidgets not found. Installing..."
    brew install wxwidgets
fi

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    print_error "CMake not found. Installing..."
    brew install cmake
fi

print_status "Dependencies check completed"

# Create build directory
BUILD_DIR="build_windows_cross"
if [ -d "$BUILD_DIR" ]; then
    print_warning "Removing existing build directory..."
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

print_status "Created build directory: $BUILD_DIR"

# Configure CMake for cross-compilation
print_status "Configuring CMake for Windows cross-compilation..."

cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=../toolchain-windows.cmake \
    -DCMAKE_CROSSCOMPILING=ON \
    -DwxWidgets_USE_STATIC=ON \
    -DwxWidgets_USE_UNICODE=ON \
    -DwxWidgets_USE_UNIVERSAL_BIN=ON \
    -G "Unix Makefiles"

if [ $? -eq 0 ]; then
    print_success "CMake configuration completed"
else
    print_error "CMake configuration failed"
    exit 1
fi

# Build the project
print_status "Building Windows executable..."

make -j$(nproc) MasoudyDictionary

if [ $? -eq 0 ]; then
    print_success "Build completed successfully!"
else
    print_error "Build failed"
    exit 1
fi

# Check if executable was created
if [ -f "bin/MasoudyDictionary.exe" ]; then
    print_success "Windows executable created: bin/MasoudyDictionary.exe"
    
    # Get file size
    FILE_SIZE=$(ls -lh bin/MasoudyDictionary.exe | awk '{print $5}')
    print_status "Executable size: $FILE_SIZE"
    
    # Check if it's a valid Windows executable
    if file bin/MasoudyDictionary.exe | grep -q "PE32+"; then
        print_success "✅ Valid Windows PE32+ executable created"
    else
        print_warning "⚠️  Executable format verification failed"
    fi
else
    print_error "Executable not found in bin/MasoudyDictionary.exe"
    exit 1
fi

# Create distribution directory
DIST_DIR="../dist_windows"
mkdir -p "$DIST_DIR"

# Copy executable and resources
cp bin/MasoudyDictionary.exe "$DIST_DIR/"
if [ -d "bin/resources" ]; then
    cp -r bin/resources "$DIST_DIR/"
fi

# Copy documentation
cp ../README.md "$DIST_DIR/" 2>/dev/null || true
cp ../LICENSE "$DIST_DIR/" 2>/dev/null || true

print_success "Distribution package created in: $DIST_DIR"

# Create a simple batch file for Windows users
cat > "$DIST_DIR/run.bat" << 'EOF'
@echo off
echo Starting Masoudy Dictionary...
MasoudyDictionary.exe
pause
EOF

print_success "Created run.bat for easy Windows execution"

# Summary
echo ""
print_success "🎉 Cross-compilation build completed successfully!"
echo ""
echo "📁 Files created:"
echo "   - $DIST_DIR/MasoudyDictionary.exe (Windows executable)"
echo "   - $DIST_DIR/run.bat (Windows launcher)"
echo "   - $DIST_DIR/README.md (Documentation)"
echo ""
echo "🚀 To use on Windows:"
echo "   1. Copy the entire '$DIST_DIR' folder to Windows"
echo "   2. Double-click 'run.bat' or 'MasoudyDictionary.exe'"
echo ""
echo "📦 Executable size: $FILE_SIZE"
echo "🔧 Built with: MinGW-w64 cross-compiler"
echo "🎯 Target: Windows x64"

cd ..
print_success "Build process completed!" 