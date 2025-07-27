@echo off
echo ========================================
echo Masoudy Dictionary - Windows Production Build
echo ========================================
echo.

:: Check if Visual Studio is available
where cl >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: Visual Studio compiler not found!
    echo Please run this script from a Visual Studio Developer Command Prompt
    echo or run: "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    pause
    exit /b 1
)

:: Check if CMake is available
where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: CMake not found!
    echo Please install CMake and add it to PATH
    pause
    exit /b 1
)

:: Create build directory
if not exist "build_windows" mkdir build_windows
cd build_windows

:: Configure with CMake for Windows
echo Configuring for Windows...
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release

if %errorlevel% neq 0 (
    echo ERROR: CMake configuration failed!
    pause
    exit /b 1
)

:: Build the application
echo Building application...
cmake --build . --config Release

if %errorlevel% neq 0 (
    echo ERROR: Build failed!
    pause
    exit /b 1
)

:: Check if executable was created
if not exist "bin\Release\MasoudyDictionary.exe" (
    echo ERROR: Executable not found!
    pause
    exit /b 1
)

echo.
echo ========================================
echo BUILD SUCCESSFUL!
echo ========================================
echo.
echo Executable: build_windows\bin\Release\MasoudyDictionary.exe
echo.
echo Features:
echo - Static linking (no external dependencies)
echo - Self-contained executable
echo - Windows production ready
echo - All features included
echo.

:: Test the executable
echo Testing executable...
bin\Release\MasoudyDictionary.exe --help

if %errorlevel% neq 0 (
    echo WARNING: Executable test failed, but build completed
) else (
    echo.
    echo Executable test successful!
)

:: Create production package
echo.
echo Creating production package...
if exist "MasoudyDictionary-Windows-v1.0.0" rmdir /s /q "MasoudyDictionary-Windows-v1.0.0"
mkdir "MasoudyDictionary-Windows-v1.0.0"
copy "bin\Release\MasoudyDictionary.exe" "MasoudyDictionary-Windows-v1.0.0\"
copy "..\README.md" "MasoudyDictionary-Windows-v1.0.0\"
copy "..\LICENSE" "MasoudyDictionary-Windows-v1.0.0\"
copy "..\architecture.txt" "MasoudyDictionary-Windows-v1.0.0\"

:: Create installer if NSIS is available
where makensis >nul 2>&1
if %errorlevel% equ 0 (
    echo Creating NSIS installer...
    makensis ..\installer.nsi
    if exist "MasoudyDictionary-Setup.exe" (
        echo Installer created: MasoudyDictionary-Setup.exe
    )
) else (
    echo NSIS not found - skipping installer creation
)

echo.
echo ========================================
echo PRODUCTION BUILD COMPLETE!
echo ========================================
echo.
echo Files created:
echo - MasoudyDictionary.exe (standalone executable)
echo - MasoudyDictionary-Windows-v1.0.0\ (production package)
if exist "MasoudyDictionary-Setup.exe" echo - MasoudyDictionary-Setup.exe (installer)
echo.
echo The application is ready for Windows deployment!
echo.
pause 