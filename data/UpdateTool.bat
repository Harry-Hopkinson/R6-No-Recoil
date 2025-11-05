@echo off
setlocal ENABLEDELAYEDEXPANSION

:: ============================================================
:: R6 No Recoil Downloader
:: ============================================================

title R6 No Recoil Downloader
color 07

echo ============================================================
echo     R6 NO RECOIL - LATEST DEVELOPMENT BUILD DOWNLOADER
echo ============================================================
echo.
echo This script will:
echo  - Download the latest R6 No Recoil build from GitHub.
echo  - Extract all ZIP folders (including nested).
echo  - Place all neatly in one folder.
echo.
echo Press CTRL+C to cancel at any time.
echo ------------------------------------------------------------
echo.

:: Set variables
set "url=https://nightly.link/Harry-Hopkinson/R6-No-Recoil/workflows/CI/main/R6NoRecoil.zip"
set "zipFile=R6NoRecoil.zip"
set "outputFolder=R6NoRecoil"

:: Step 1: Check for PowerShell
where powershell >nul 2>nul
if errorlevel 1 (
    powershell -Command "Write-Host '[ERROR] PowerShell not found! Please run this script on Windows 10/11.' -ForegroundColor Red"
    pause
    exit /b 1
)

powershell -Command "Write-Host '[INFO] PowerShell detected.' -ForegroundColor Green"
echo ------------------------------------------------------------
powershell -Command "Write-Host '[STEP 1/5] Downloading the latest build...' -ForegroundColor Cyan"
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
powershell -Command "Write-Host '[STEP 2/5] Extracting main ZIP...' -ForegroundColor Cyan"
if exist "%outputFolder%" (
    echo [INFO] Removing old "%outputFolder%" folder...
    rmdir /s /q "%outputFolder%"
)

powershell -Command ^
    "try { Expand-Archive -Path '%zipFile%' -DestinationPath '%outputFolder%' -Force; Write-Host 'Primary extraction complete.' -ForegroundColor Green } catch { Write-Host 'Extraction failed:' $_.Exception.Message -ForegroundColor Red; exit 1 }"

if not exist "%outputFolder%" (
    powershell -Command "Write-Host '[ERROR] Extraction failed — output folder missing.' -ForegroundColor Red"
    pause
    exit /b 1
)

echo ------------------------------------------------------------
powershell -Command "Write-Host '[STEP 3/5] Checking for nested ZIP or subfolder...' -ForegroundColor Cyan"
set "innerZip=%outputFolder%\R6NoRecoil.zip"

if exist "%innerZip%" (
    powershell -Command "Write-Host '[INFO] Nested ZIP found, extracting...' -ForegroundColor Yellow"
    powershell -Command ^
        "try { Expand-Archive -Path '%innerZip%' -DestinationPath '%outputFolder%\inner' -Force; Write-Host 'Inner ZIP extracted successfully.' -ForegroundColor Green } catch { Write-Host 'Failed to extract inner ZIP:' $_.Exception.Message -ForegroundColor Red; exit 1 }"
    xcopy "%outputFolder%\inner\*" "%outputFolder%\" /E /H /Y >nul
    del "%innerZip%"
    rmdir /s /q "%outputFolder%\inner"
) else (
    echo [INFO] No nested ZIP found.
)

:: Handle nested folder structure (excluding assets)
for /d %%F in ("%outputFolder%\*") do (
    if exist "%%F\*" (
        if /I not "%%~nxF"=="assets" (
            powershell -Command "Write-Host '[INFO] Moving files from subfolder %%~nxF up...' -ForegroundColor Yellow"
            xcopy "%%F\*" "%outputFolder%\" /E /H /Y >nul
            rmdir /s /q "%%F"
            goto Flattened
        )
    )
)
:Flattened

echo ------------------------------------------------------------
powershell -Command "Write-Host '[STEP 4/5] Preserving assets structure...' -ForegroundColor Cyan"
if exist "%outputFolder%\assets" (
    if exist "%outputFolder%\assets\operators" powershell -Command "Write-Host '  Found operators folder — keeping it.' -ForegroundColor Green"
    if exist "%outputFolder%\assets\weapons" powershell -Command "Write-Host '  Found weapons folder — keeping it.' -ForegroundColor Green"
) else (
    echo [INFO] No assets folder found — skipping.
)

echo ------------------------------------------------------------
powershell -Command "Write-Host '[STEP 5/5] Cleaning up...' -ForegroundColor Cyan"
if exist "%zipFile%" del "%zipFile%"
echo [INFO] Removed temporary ZIP file.

echo.
powershell -Command "Write-Host 'All files extracted successfully!' -ForegroundColor Green"
echo Location: "%cd%\%outputFolder%"
echo ------------------------------------------------------------
echo.

set /p openfolder="Would you like to open the folder now? (Y/N): "
if /I "%openfolder%"=="Y" (
    start "" "%cd%\%outputFolder%"
)

echo ------------------------------------------------------------
powershell -Command "Write-Host 'Script completed successfully. Thank you for using the R6 No Recoil Downloader!' -ForegroundColor Magenta"
echo ------------------------------------------------------------
pause
endlocal
