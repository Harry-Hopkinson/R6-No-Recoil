@echo off
setlocal ENABLEDELAYEDEXPANSION

:: ============================================================
:: R6 No Recoil Downloader
:: ============================================================

title R6 No Recoil Downloader
color 07

echo ============================================================
echo     R6 NO RECOIL - LATEST RELEASE BUILD DOWNLOADER
echo ============================================================
echo.
echo This script will:
echo  - Download the latest R6 No Recoil build from GitHub.
echo  - Extract it into one clean folder.
echo  - Preserve the "assets" folder.
echo  - Replace WeaponData.json automatically.
echo.
echo Press CTRL+C to cancel at any time.
echo ------------------------------------------------------------
echo.

:: Variables
set "url=https://github.com/Harry-Hopkinson/R6-No-Recoil/releases/latest/download/R6NoRecoil.zip"
set "zipFile=R6NoRecoil.zip"
set "outputFolder=R6NoRecoil"
set "weaponData=WeaponData.json"

:: Step 1: Check for PowerShell
where powershell >nul 2>nul
if errorlevel 1 (
    powershell -Command "Write-Host '[ERROR] PowerShell not found! Please run this script on Windows 10/11.' -ForegroundColor Red"
    pause
    exit /b 1
)

powershell -Command "Write-Host '[INFO] PowerShell detected.' -ForegroundColor Green"
echo ------------------------------------------------------------
powershell -Command "Write-Host '[STEP 1/4] Downloading latest build...' -ForegroundColor Cyan"
echo URL: %url%
echo.

:: Step 2: Download ZIP
powershell -Command ^
    "$ProgressPreference='SilentlyContinue';" ^
    "try {" ^
    "  Invoke-WebRequest -Uri '%url%' -OutFile '%zipFile%' -UseBasicParsing;" ^
    "  Write-Host 'Download completed successfully.' -ForegroundColor Green" ^
    "} catch { Write-Host 'Download failed:' $_.Exception.Message -ForegroundColor Red; exit 1 }"

if not exist "%zipFile%" (
    powershell -Command "Write-Host '[ERROR] Failed to download ZIP file. Check your connection or URL.' -ForegroundColor Red"
    pause
    exit /b 1
)

echo ------------------------------------------------------------
powershell -Command "Write-Host '[STEP 2/4] Extracting ZIP...' -ForegroundColor Cyan"
if exist "%outputFolder%" (
    echo [INFO] Removing old "%outputFolder%" folder...
    rmdir /s /q "%outputFolder%"
)

powershell -Command ^
    "try { Expand-Archive -Path '%zipFile%' -DestinationPath '%outputFolder%' -Force; Write-Host 'Extraction complete.' -ForegroundColor Green } catch { Write-Host 'Extraction failed:' $_.Exception.Message -ForegroundColor Red; exit 1 }"

if not exist "%outputFolder%" (
    powershell -Command "Write-Host '[ERROR] Extraction failed — output folder missing.' -ForegroundColor Red"
    pause
    exit /b 1
)

echo ------------------------------------------------------------
powershell -Command "Write-Host '[STEP 3/4] Replacing WeaponData.json...' -ForegroundColor Cyan"

:: Replace WeaponData.json
if exist "%weaponData%" (
    echo [INFO] Found local WeaponData.json, copying to new folder...
    copy /Y "%weaponData%" "%outputFolder%\%weaponData%" >nul
    if exist "%outputFolder%\%weaponData%" (
        powershell -Command "Write-Host '[SUCCESS] WeaponData.json replaced successfully.' -ForegroundColor Green"
    ) else (
        powershell -Command "Write-Host '[WARNING] Failed to copy WeaponData.json to new folder.' -ForegroundColor Yellow"
    )
) else (
    powershell -Command "Write-Host '[WARNING] No local WeaponData.json found in script directory.' -ForegroundColor Yellow"
)

echo ------------------------------------------------------------
powershell -Command "Write-Host '[STEP 4/4] Cleaning up...' -ForegroundColor Cyan"
if exist "%zipFile%" del "%zipFile%"
echo [INFO] Removed temporary ZIP file.

echo.
powershell -Command "Write-Host 'All files extracted and WeaponData.json updated successfully!' -ForegroundColor Green"
echo Location: "%cd%\%outputFolder%"
echo ------------------------------------------------------------
echo.

set /p runnow="Would you like to run R6 No Recoil now? (Y/N): "
if /I "%runnow%"=="Y" (
    start "" "%cd%\%outputFolder%\R6NoRecoil.exe"
)

echo ------------------------------------------------------------
powershell -Command "Write-Host 'Script completed successfully. Thank you for using the R6 No Recoil Downloader!' -ForegroundColor Magenta"
echo ------------------------------------------------------------
pause
endlocal
