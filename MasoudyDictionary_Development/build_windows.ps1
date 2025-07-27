# Masoudy Dictionary - Windows Production Build Script
# PowerShell version with enhanced error handling

Write-Host "========================================" -ForegroundColor Green
Write-Host "Masoudy Dictionary - Windows Production Build" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""

# Check if running on Windows
if ($env:OS -notlike "*Windows*") {
    Write-Host "ERROR: This script must be run on Windows!" -ForegroundColor Red
    exit 1
}

# Check if Visual Studio is available
$vsPath = Get-Command cl -ErrorAction SilentlyContinue
if (-not $vsPath) {
    Write-Host "ERROR: Visual Studio compiler not found!" -ForegroundColor Red
    Write-Host "Please run this script from a Visual Studio Developer Command Prompt" -ForegroundColor Yellow
    Write-Host "or run: 'C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat'" -ForegroundColor Yellow
    Read-Host "Press Enter to exit"
    exit 1
}

# Check if CMake is available
$cmakePath = Get-Command cmake -ErrorAction SilentlyContinue
if (-not $cmakePath) {
    Write-Host "ERROR: CMake not found!" -ForegroundColor Red
    Write-Host "Please install CMake and add it to PATH" -ForegroundColor Yellow
    Read-Host "Press Enter to exit"
    exit 1
}

# Create build directory
$buildDir = "build_windows"
if (-not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}
Set-Location $buildDir

# Configure with CMake for Windows
Write-Host "Configuring for Windows..." -ForegroundColor Cyan
$cmakeResult = cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: CMake configuration failed!" -ForegroundColor Red
    Read-Host "Press Enter to exit"
    exit 1
}

# Build the application
Write-Host "Building application..." -ForegroundColor Cyan
$buildResult = cmake --build . --config Release
if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Build failed!" -ForegroundColor Red
    Read-Host "Press Enter to exit"
    exit 1
}

# Check if executable was created
$exePath = "bin\Release\MasoudyDictionary.exe"
if (-not (Test-Path $exePath)) {
    Write-Host "ERROR: Executable not found!" -ForegroundColor Red
    Read-Host "Press Enter to exit"
    exit 1
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "BUILD SUCCESSFUL!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""
Write-Host "Executable: $buildDir\$exePath" -ForegroundColor White
Write-Host ""
Write-Host "Features:" -ForegroundColor Yellow
Write-Host "- Static linking (no external dependencies)" -ForegroundColor White
Write-Host "- Self-contained executable" -ForegroundColor White
Write-Host "- Windows production ready" -ForegroundColor White
Write-Host "- All features included" -ForegroundColor White
Write-Host ""

# Test the executable
Write-Host "Testing executable..." -ForegroundColor Cyan
$testResult = & $exePath --help
if ($LASTEXITCODE -ne 0) {
    Write-Host "WARNING: Executable test failed, but build completed" -ForegroundColor Yellow
} else {
    Write-Host ""
    Write-Host "Executable test successful!" -ForegroundColor Green
}

# Create production package
Write-Host ""
Write-Host "Creating production package..." -ForegroundColor Cyan
$packageDir = "MasoudyDictionary-Windows-v1.0.0"
if (Test-Path $packageDir) {
    Remove-Item $packageDir -Recurse -Force
}
New-Item -ItemType Directory -Path $packageDir | Out-Null

Copy-Item $exePath $packageDir\
Copy-Item "..\README.md" $packageDir\
Copy-Item "..\LICENSE" $packageDir\
Copy-Item "..\architecture.txt" $packageDir\

# Create installer if NSIS is available
$nsisPath = Get-Command makensis -ErrorAction SilentlyContinue
if ($nsisPath) {
    Write-Host "Creating NSIS installer..." -ForegroundColor Cyan
    $nsisResult = makensis ..\installer.nsi
    if (Test-Path "MasoudyDictionary-Setup.exe") {
        Write-Host "Installer created: MasoudyDictionary-Setup.exe" -ForegroundColor Green
    }
} else {
    Write-Host "NSIS not found - skipping installer creation" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "PRODUCTION BUILD COMPLETE!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""
Write-Host "Files created:" -ForegroundColor Yellow
Write-Host "- MasoudyDictionary.exe (standalone executable)" -ForegroundColor White
Write-Host "- $packageDir\ (production package)" -ForegroundColor White
if (Test-Path "MasoudyDictionary-Setup.exe") {
    Write-Host "- MasoudyDictionary-Setup.exe (installer)" -ForegroundColor White
}
Write-Host ""
Write-Host "The application is ready for Windows deployment!" -ForegroundColor Green
Write-Host ""
Read-Host "Press Enter to exit" 