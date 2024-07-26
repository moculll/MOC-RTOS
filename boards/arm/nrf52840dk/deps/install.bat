@echo off
setlocal enabledelayedexpansion

set "SCRIPT_DIR=%~dp0"
set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

set "EXTRACT_DIR=%SCRIPT_DIR%"

if not exist "%EXTRACT_DIR%" mkdir "%EXTRACT_DIR%"

set "URL="
set "TYPE="

for /f "delims=" %%i in ('type "%SCRIPT_DIR%\tools_win.dep"') do (
    if not defined URL (
        set "URL=%%i"
    ) else if not defined TYPE (
        set "TYPE=%%i"
        
        for %%f in ("!URL!") do set "FILE_NAME=%%~nxf"
        
        set "FILE_NAME=!FILE_NAME:/=\!"

        set "FULL_URL=!URL!!TYPE!"
        echo Downloading !FULL_URL! to !SCRIPT_DIR!\!FILE_NAME!!TYPE!

        powershell -Command "Invoke-WebRequest -Uri '!FULL_URL!' -OutFile '!SCRIPT_DIR!\!FILE_NAME!!TYPE!'"

        if exist "!SCRIPT_DIR!\!FILE_NAME!!TYPE!" (
            echo Extracting !SCRIPT_DIR!\!FILE_NAME!!TYPE! to %EXTRACT_DIR%
            if "!TYPE!"==".zip" (
                powershell -Command "Expand-Archive -Path '!SCRIPT_DIR!\!FILE_NAME!!TYPE!' -DestinationPath '%EXTRACT_DIR%'"
            ) else if "!TYPE!"==".tar.bz2" (
                tar -xjf "!SCRIPT_DIR!\!FILE_NAME!!TYPE!" -C "%EXTRACT_DIR%"
            ) else if "!TYPE!"==".tar.gz" (
                tar -xzf "!SCRIPT_DIR!\!FILE_NAME!!TYPE!" -C "%EXTRACT_DIR%"
            ) else (
                echo Unsupported file type: !TYPE!
            )
        ) else (
            echo Failed to download !FULL_URL!
        )

        set "URL="
        set "TYPE="
    )
)

echo Downloaded Deps successfully, run [scripts/build.bat BOARD=nrf52840dk] to start!
endlocal
