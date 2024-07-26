@echo off
setlocal enabledelayedexpansion

if "%~1" == "" (
    echo Usage:
    echo   %~nx0 BOARD=nrf52840dk
    exit /b 1
)

set "SCRIPT_DIR=%~dp0"
set "MOCROOT_DIR=%SCRIPT_DIR%\.."

for %%A in (%*) do (
    set "ARG=%%~A"
    set "BOARD=!ARG:*BOARD=!"

    if "!BOARD!" neq "!ARG!" (
        set "BOARD=!BOARD:~1!"
    )
)

set "OUTPUT_DIR=%MOCROOT_DIR%\out\%BOARD%"

cd /d "%MOCROOT_DIR%"

set "BOARDS_ARRAY=nrf52840dk,another_board,yet_another_board"

for %%i in (%BOARDS_ARRAY%) do (
    set "BOARD=%%i"

    set "TARGET_DIR="
    for /r "%MOCROOT_DIR%\boards" %%j in (.) do (
        if /i "%%~nxj"=="!BOARD!" (
            set "TARGET_DIR=%%j"
            goto Found
        )
    )
    :Found
    if defined TARGET_DIR (
        echo !BOARD! support found at !TARGET_DIR!, getting board dependency...
        call "!TARGET_DIR!\deps\env.bat"
        
        :: Clean previous build
        if exist "%MOCROOT_DIR%\build" rmdir /s /q "%MOCROOT_DIR%\build"
        echo [CLEAN] Removed previous build directory
        ninja --version
        arm-none-eabi-gcc -v

        cmake -S . -B build -DBOARD=%BOARD% -G Ninja
        cmake --build build
        
        if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"
        
        copy "%MOCROOT_DIR%\build\MOC-RTOS.hex" "%OUTPUT_DIR%\" > nul
        copy "%MOCROOT_DIR%\build\%BOARD%.map" "%OUTPUT_DIR%\" > nul
        copy "%MOCROOT_DIR%\build\MOC-RTOS.bin" "%OUTPUT_DIR%\" > nul
        
    ) else (
        echo !BOARD! support not found, skip...
    )
)

echo Build !BOARD! done.

endlocal
