@echo off

set "SCRIPT_DIR=%~dp0"
set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

set "TOOLS_FILE=%SCRIPT_DIR%\tools_win.dep"

for /f "delims=" %%i in ('type "%TOOLS_FILE%"') do (
    if not defined URL (
        set "URL=%%i"
    ) else if not defined TYPE (
        set "TYPE=%%i"

        for %%f in ("!URL!") do (
            set "FILE_NAME=%%~nxf"
            set "FILE_NAME=!FILE_NAME:/=\!"
        )

        set "NEW_PATH=%SCRIPT_DIR%\!FILE_NAME!\bin"
        echo Adding to PATH: !NEW_PATH!
        
        REM Set temporary environment variable
        set "PATH=!PATH!;!NEW_PATH!"

        set "URL="
        set "TYPE="
    )
)
set "NEW_PATH=%SCRIPT_DIR%\cmake-3.30.1-windows-x86_64\bin"
set "PATH=!PATH!;!NEW_PATH!"
set "NEW_PATH=%SCRIPT_DIR%\gcc-arm-none-eabi-10.3-2021.10\bin"
set "PATH=!PATH!;!NEW_PATH!"
set "NEW_PATH=%SCRIPT_DIR%"
set "PATH=!PATH!;!NEW_PATH!"
echo %PATH%
echo Done setting temporary environment variables.

