@echo off
setlocal enabledelayedexpansion

echo ==========================================
echo Masoudy Dictionary Windows Production Build
echo Version: 1.0.0
echo ==========================================

:: Configuration
set PROJECT_NAME=MasoudyDictionary
set VERSION=1.0.0
set BUILD_DIR=build_windows_production
set PRODUCTION_DIR=MasoudyDictionary_Production\Versions\Masoudy-Dictionary-V-1.0

:: Check if we're on Windows
if not "%OS%"=="Windows_NT" (
    echo ERROR: This script is for Windows only!
    pause
    exit /b 1
)

:: Check for Visual Studio
where cl >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: Visual Studio not found!
    echo Please install Visual Studio 2022 with C++ development tools
    pause
    exit /b 1
)

:: Check for CMake
where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: CMake not found!
    echo Please install CMake from https://cmake.org/
    pause
    exit /b 1
)

:: Check for wxWidgets
if not exist "C:\wxWidgets" (
    echo WARNING: wxWidgets not found in C:\wxWidgets
    echo Please install wxWidgets or set WXWIN environment variable
    echo Download from: https://www.wxwidgets.org/
    pause
)

:: Clean previous build
echo [INFO] Cleaning previous build...
if exist %BUILD_DIR% rmdir /s /q %BUILD_DIR%
mkdir %BUILD_DIR%

:: Create build directory
echo [INFO] Creating build directory...
cd %BUILD_DIR%

:: Configure with CMake
echo [INFO] Configuring with CMake...
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release

if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed!
    pause
    exit /b 1
)

:: Build the application
echo [INFO] Building application...
cmake --build . --config Release --parallel

if %errorlevel% neq 0 (
    echo [ERROR] Build failed!
    pause
    exit /b 1
)

:: Check if build was successful
if not exist "bin\Release\%PROJECT_NAME%.exe" (
    echo [ERROR] Build failed! Executable not found.
    pause
    exit /b 1
)

echo [SUCCESS] Build completed successfully!

:: Create production directory structure
echo [INFO] Creating production package...

:: Clean and recreate production directory
if exist "..\%PRODUCTION_DIR%" rmdir /s /q "..\%PRODUCTION_DIR%"
mkdir "..\%PRODUCTION_DIR%"

:: Copy executable
echo [INFO] Copying executable...
copy "bin\Release\%PROJECT_NAME%.exe" "..\%PRODUCTION_DIR%\"

:: Copy documentation
echo [INFO] Copying documentation...
xcopy "..\docs" "..\%PRODUCTION_DIR%\docs\" /E /I /Y

:: Copy resources
echo [INFO] Copying resources...
mkdir "..\%PRODUCTION_DIR%\resources"
if exist "..\icon.ico" copy "..\icon.ico" "..\%PRODUCTION_DIR%\resources\"

:: Copy samples
echo [INFO] Copying samples...
mkdir "..\%PRODUCTION_DIR%\samples"
if exist "..\config_sample.ini" copy "..\config_sample.ini" "..\%PRODUCTION_DIR%\samples\"

:: Copy license and readme
echo [INFO] Copying license and readme...
copy "..\LICENSE" "..\%PRODUCTION_DIR%\"
copy "..\README.md" "..\%PRODUCTION_DIR%\README.txt"

:: Create installation guide
echo [INFO] Creating installation guide...
(
echo ==========================================
echo Masoudy Dictionary - Installation Guide
echo ==========================================
echo.
echo Version: %VERSION%
echo Build Date: %date% %time%
echo.
echo SYSTEM REQUIREMENTS:
echo - Windows 10/11 ^(64-bit^)
echo - 4GB RAM minimum
echo - 50MB free disk space
echo - Administrator privileges ^(for full features^)
echo.
echo INSTALLATION METHODS:
echo.
echo 1. PORTABLE VERSION:
echo    - Extract the ZIP file to any location
echo    - Run MasoudyDictionary.exe directly
echo    - No installation required
echo.
echo 2. INSTALLER VERSION:
echo    - Run MasoudyDictionary-Setup.exe
echo    - Follow the installation wizard
echo    - Application will be installed to Program Files
echo.
echo FEATURES:
echo - Advanced Dictionary Application
echo - Professional GUI Interface
echo - Screen Sharing Capabilities
echo - Network Scanning Tools
echo - Stealth Mode Operations
echo - Exam-Proof Features
echo.
echo SECURITY NOTICE:
echo This application includes advanced cybersecurity features.
echo Use responsibly and in accordance with applicable laws.
echo.
echo SUPPORT:
echo For technical support, refer to the documentation in the docs/ folder.
echo.
echo LICENSE:
echo See LICENSE file for terms and conditions.
) > "..\%PRODUCTION_DIR%\INSTALLATION_GUIDE.txt"

:: Create build script for Windows
echo [INFO] Creating Windows build script...
(
echo @echo off
echo echo Building Masoudy Dictionary Installer...
echo echo.
echo.
echo REM Check if NSIS is installed
echo where makensis ^>nul 2^>^&1
echo if %%errorlevel%% neq 0 ^(
echo     echo ERROR: NSIS not found. Please install NSIS to create installer.
echo     echo Download from: https://nsis.sourceforge.io/
echo     pause
echo     exit /b 1
echo ^)
echo.
echo REM Create installer
echo echo Creating installer...
echo makensis installer.nsi
echo.
echo if %%errorlevel%% equ 0 ^(
echo     echo.
echo     echo SUCCESS: Installer created successfully!
echo     echo Installer: MasoudyDictionary-Setup.exe
echo ^) else ^(
echo     echo.
echo     echo ERROR: Failed to create installer.
echo ^)
echo.
echo pause
) > "..\%PRODUCTION_DIR%\build_installer.bat"

:: Create NSIS installer script
echo [INFO] Creating NSIS installer script...
(
echo ; Masoudy Dictionary Installer Script
echo ; NSIS Installer Configuration
echo.
echo !define APP_NAME "Masoudy Dictionary"
echo !define APP_VERSION "%VERSION%"
echo !define APP_PUBLISHER "Masoudy Research"
echo !define APP_EXE "%PROJECT_NAME%.exe"
echo !define APP_ICON "resources\icon.ico"
echo.
echo ; Include modern UI
echo !include "MUI2.nsh"
echo.
echo ; General
echo Name "$^^{APP_NAME^}$"
echo OutFile "MasoudyDictionary-Setup.exe"
echo InstallDir "$PROGRAMFILES64\$^^{APP_NAME^}$"
echo InstallDirRegKey HKCU "Software\$^^{APP_NAME^}$" ""
echo.
echo ; Request application privileges
echo RequestExecutionLevel admin
echo.
echo ; Interface Settings
echo !define MUI_ABORTWARNING
echo !define MUI_ICON "$^^{APP_ICON^}$"
echo !define MUI_UNICON "$^^{APP_ICON^}$"
echo.
echo ; Pages
echo !insertmacro MUI_PAGE_WELCOME
echo !insertmacro MUI_PAGE_LICENSE "LICENSE"
echo !insertmacro MUI_PAGE_DIRECTORY
echo !insertmacro MUI_PAGE_INSTFILES
echo !insertmacro MUI_PAGE_FINISH
echo.
echo !insertmacro MUI_UNPAGE_WELCOME
echo !insertmacro MUI_UNPAGE_CONFIRM
echo !insertmacro MUI_UNPAGE_INSTFILES
echo !insertmacro MUI_UNPAGE_FINISH
echo.
echo ; Languages
echo !insertmacro MUI_LANGUAGE "English"
echo.
echo ; Installer Sections
echo Section "Main Application" SecMain
echo     SetOutPath "$INSTDIR"
echo     .
echo     ; Main executable
echo     File "$^^{APP_EXE^}$"
echo     .
echo     ; Resources
echo     SetOutPath "$INSTDIR\resources"
echo     File "resources\*.*"
echo     .
echo     ; Documentation
echo     SetOutPath "$INSTDIR\docs"
echo     File /r "docs\*.*"
echo     .
echo     ; Samples
echo     SetOutPath "$INSTDIR\samples"
echo     File "samples\*.*"
echo     .
echo     ; License and readme
echo     File "LICENSE"
echo     File "README.txt"
echo     File "INSTALLATION_GUIDE.txt"
echo     .
echo     ; Create uninstaller
echo     WriteUninstaller "$INSTDIR\Uninstall.exe"
echo     .
echo     ; Registry information for add/remove programs
echo     WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$^^{APP_NAME^}$" "DisplayName" "$^^{APP_NAME^}$"
echo     WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$^^{APP_NAME^}$" "UninstallString" "$^"$INSTDIR\Uninstall.exe$^""
echo     WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$^^{APP_NAME^}$" "DisplayIcon" "$INSTDIR\$^^{APP_EXE^}$"
echo     WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$^^{APP_NAME^}$" "Publisher" "$^^{APP_PUBLISHER^}$"
echo     WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$^^{APP_NAME^}$" "DisplayVersion" "$^^{APP_VERSION^}$"
echo     WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$^^{APP_NAME^}$" "NoModify" 1
echo     WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$^^{APP_NAME^}$" "NoRepair" 1
echo     .
echo     ; Create start menu shortcut
echo     CreateDirectory "$SMPROGRAMS\$^^{APP_NAME^}$"
echo     CreateShortCut "$SMPROGRAMS\$^^{APP_NAME^}$\$^^{APP_NAME^}$.lnk" "$INSTDIR\$^^{APP_EXE^}$"
echo     CreateShortCut "$SMPROGRAMS\$^^{APP_NAME^}$\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
echo     .
echo     ; Create desktop shortcut
echo     CreateShortCut "$DESKTOP\$^^{APP_NAME^}$.lnk" "$INSTDIR\$^^{APP_EXE^}$"
echo SectionEnd
echo.
echo ; Uninstaller Section
echo Section "Uninstall"
echo     ; Remove files
echo     Delete "$INSTDIR\$^^{APP_EXE^}$"
echo     RMDir /r "$INSTDIR\resources"
echo     RMDir /r "$INSTDIR\docs"
echo     RMDir /r "$INSTDIR\samples"
echo     Delete "$INSTDIR\LICENSE"
echo     Delete "$INSTDIR\README.txt"
echo     Delete "$INSTDIR\INSTALLATION_GUIDE.txt"
echo     Delete "$INSTDIR\Uninstall.exe"
echo     .
echo     ; Remove directories
echo     RMDir "$INSTDIR"
echo     .
echo     ; Remove start menu shortcuts
echo     Delete "$SMPROGRAMS\$^^{APP_NAME^}$\$^^{APP_NAME^}$.lnk"
echo     Delete "$SMPROGRAMS\$^^{APP_NAME^}$\Uninstall.lnk"
echo     RMDir "$SMPROGRAMS\$^^{APP_NAME^}$"
echo     .
echo     ; Remove desktop shortcut
echo     Delete "$DESKTOP\$^^{APP_NAME^}$.lnk"
echo     .
echo     ; Remove registry keys
echo     DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$^^{APP_NAME^}$"
echo     DeleteRegKey HKCU "Software\$^^{APP_NAME^}$"
echo SectionEnd
) > "..\%PRODUCTION_DIR%\installer.nsi"

:: Create package info
echo [INFO] Creating package information...
(
echo ==========================================
echo Masoudy Dictionary Production Package
echo ==========================================
echo.
echo Package Information:
echo - Version: %VERSION%
echo - Build Date: %date% %time%
echo - Build Platform: Windows x64
echo - Executable: %PROJECT_NAME%.exe
echo.
echo Contents:
echo - %PROJECT_NAME%.exe ^(Main executable^)
echo - docs/ ^(Documentation^)
echo - resources/ ^(Application resources^)
echo - samples/ ^(Sample configurations^)
echo - LICENSE ^(License file^)
echo - README.txt ^(Readme file^)
echo - INSTALLATION_GUIDE.txt ^(Installation instructions^)
echo - installer.nsi ^(NSIS installer script^)
echo - build_installer.bat ^(Windows installer build script^)
echo.
echo Features:
echo - Professional GUI Interface
echo - Advanced Dictionary Application
echo - Screen Sharing Capabilities
echo - Network Scanning Tools
echo - Stealth Mode Operations
echo - Exam-Proof Features
echo - Self-contained ^(No external dependencies^)
echo - Static linking enabled
echo.
echo Installation:
echo 1. Portable: Run %PROJECT_NAME%.exe directly
echo 2. Installer: Use build_installer.bat to create installer
echo.
echo Security:
echo - Advanced cybersecurity features
echo - Stealth and evasion capabilities
echo - Exam-proof operation modes
echo - Professional-grade security
echo.
echo Support:
echo - Documentation included in docs/ folder
echo - Sample configurations in samples/ folder
echo - Installation guide provided
echo.
echo License:
echo - See LICENSE file for terms and conditions
) > "..\%PRODUCTION_DIR%\PACKAGE_INFO.txt"

:: Create final package
echo [INFO] Creating final package...
cd "..\%PRODUCTION_DIR%"

:: Create ZIP package
echo [INFO] Creating ZIP package...
powershell -command "Compress-Archive -Path * -DestinationPath ..\MasoudyDictionary-V1.0-Portable.zip -Force"

:: Create installer (if NSIS is available)
where makensis >nul 2>&1
if %errorlevel% equ 0 (
    echo [INFO] Creating installer...
    makensis installer.nsi
    if %errorlevel% equ 0 (
        echo [SUCCESS] Installer created successfully!
        copy MasoudyDictionary-Setup.exe ..\
    ) else (
        echo [WARNING] Failed to create installer
    )
) else (
    echo [WARNING] NSIS not found - installer not created
)

echo [SUCCESS] Production package created successfully!
echo [INFO] Package location: %PRODUCTION_DIR%
echo [INFO] ZIP package: MasoudyDictionary_Production\MasoudyDictionary-V1.0-Portable.zip

:: Display package information
echo.
echo ==========================================
echo PRODUCTION PACKAGE SUMMARY
echo ==========================================
echo Package Location: %PRODUCTION_DIR%
echo Executable: %PROJECT_NAME%.exe
echo Version: %VERSION%
echo Build Date: %date% %time%
echo.
echo Files included:
dir /b
echo.
echo ==========================================
echo PRODUCTION BUILD COMPLETED SUCCESSFULLY!
echo ==========================================

pause 