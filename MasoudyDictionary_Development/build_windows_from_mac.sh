#!/bin/bash

echo "========================================"
echo "Masoudy Dictionary - Windows Cross-Compilation"
echo "========================================"
echo

echo "[1/6] Checking cross-compilation tools..."
if ! command -v x86_64-w64-mingw32-g++ &> /dev/null; then
    echo "ERROR: MinGW-w64 not found!"
    echo "Please install: brew install mingw-w64"
    exit 1
fi

if ! command -v cmake &> /dev/null; then
    echo "ERROR: CMake not found!"
    echo "Please install: brew install cmake"
    exit 1
fi

echo "✅ Cross-compilation tools found"
echo

echo "[2/6] Cleaning previous builds..."
rm -rf build_windows
rm -rf MasoudyInstallerContent
mkdir build_windows
cd build_windows

echo "[3/6] Building Windows application directly..."
cd ..

# Compile Windows application directly with MinGW
echo "Compiling MasoudyDictionary.exe..."
x86_64-w64-mingw32-g++ \
    -std=c++17 \
    -DWIN32 -D_WIN32 -D_WINDOWS -D_UNICODE -DUNICODE \
    -DWIN32_LEAN_AND_MEAN -DNOMINMAX -D_WIN32_WINNT=0x0601 \
    -static -static-libgcc -static-libstdc++ \
    -O2 -s \
    -o MasoudyDictionary.exe \
    src/windows_app.cpp \
    -lcomctl32 -lshlwapi -luser32 -lgdi32 -lkernel32 -lshell32

if [ $? -ne 0 ]; then
    echo "ERROR: Compilation failed!"
    exit 1
fi

echo "[4/6] Preparing installer content..."
mkdir -p MasoudyInstallerContent

# Copy main executable
if [ -f "MasoudyDictionary.exe" ]; then
    cp MasoudyDictionary.exe MasoudyInstallerContent/
    echo "✅ Main executable copied"
else
    echo "ERROR: Main executable not found!"
    exit 1
fi

# Copy configuration files
cp -r config/ MasoudyInstallerContent/ 2>/dev/null || mkdir -p MasoudyInstallerContent/config
cp -r data/ MasoudyInstallerContent/ 2>/dev/null || mkdir -p MasoudyInstallerContent/data
cp -r lang/ MasoudyInstallerContent/ 2>/dev/null || mkdir -p MasoudyInstallerContent/lang
cp -r plugins/ MasoudyInstallerContent/ 2>/dev/null || mkdir -p MasoudyInstallerContent/plugins
cp -r themes/ MasoudyInstallerContent/ 2>/dev/null || mkdir -p MasoudyInstallerContent/themes
cp -r resources/ MasoudyInstallerContent/ 2>/dev/null || mkdir -p MasoudyInstallerContent/resources
cp -r docs/ MasoudyInstallerContent/ 2>/dev/null || mkdir -p MasoudyInstallerContent/docs

# Create logs directory
mkdir -p MasoudyInstallerContent/logs

# Copy additional files
cp LICENSE MasoudyInstallerContent/ 2>/dev/null || echo "# MIT License" > MasoudyInstallerContent/LICENSE
cp version.txt MasoudyInstallerContent/ 2>/dev/null || echo "v1.0.0" > MasoudyInstallerContent/version.txt
cp README.txt MasoudyInstallerContent/ 2>/dev/null || echo "Masoudy Dictionary" > MasoudyInstallerContent/README.txt

echo "[5/6] Creating professional installer..."
if command -v makensis &> /dev/null; then
    makensis installer.nsi
    if [ $? -eq 0 ]; then
        echo "✅ Professional installer created: MasoudyDictionary_Professional_Installer.exe"
    else
        echo "⚠️  NSIS compilation failed, installer not created"
    fi
else
    echo "⚠️  NSIS not found. Installer will need to be created on Windows."
    echo "   The application files are ready in MasoudyInstallerContent/"
fi

echo "[6/6] Build completed successfully!"
echo
echo "========================================"
echo "BUILD SUMMARY"
echo "========================================"
echo

# Show file sizes
if [ -f "MasoudyInstallerContent/MasoudyDictionary.exe" ]; then
    EXE_SIZE=$(stat -f%z "MasoudyInstallerContent/MasoudyDictionary.exe")
    echo "Application: MasoudyDictionary.exe"
    echo "Size: $EXE_SIZE bytes"
    echo
fi

if [ -f "MasoudyDictionary_Professional_Installer.exe" ]; then
    INSTALLER_SIZE=$(stat -f%z "MasoudyDictionary_Professional_Installer.exe")
    echo "Installer: MasoudyDictionary_Professional_Installer.exe"
    echo "Size: $INSTALLER_SIZE bytes"
    echo
fi

echo "Installer Content: MasoudyInstallerContent/"
echo "Total Files: $(find MasoudyInstallerContent -type f | wc -l)"
echo

echo "Next Steps:"
echo "1. Copy MasoudyInstallerContent/ to Windows machine"
echo "2. Run MasoudyDictionary.exe to test"
echo "3. Use NSIS on Windows to create installer: makensis installer.nsi"
echo "4. Distribute the professional installer"
echo

echo "========================================"
echo "Cross-compilation completed successfully!"
echo "========================================" 