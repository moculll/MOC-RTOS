@echo off
setlocal enabledelayedexpansion

set "SCRIPT_DIR=%~dp0"
set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

for /f "tokens=1,* delims==" %%i in ('findstr "BOARDS=" "%SCRIPT_DIR%\..\config\boards.cfg"') do set "BOARDS_ARRAY=%%j"
set "BOARDS_ARRAY=%BOARDS_ARRAY:[=%"
set "BOARDS_ARRAY=%BOARDS_ARRAY:]=%"
echo get [%BOARDS_ARRAY%], installing dependency...

set "BOARDS_ARRAY=%BOARDS_ARRAY:,= %"
setlocal enabledelayedexpansion
set "SPILT="
for %%i in (%BOARDS_ARRAY%) do (
    set "item=%%i"
    set "SPILT=!SPILT!!item: =!!item: =!\n"
)

for %%i in (%BOARDS_ARRAY%) do (
    set "BOARD=%%i"
    
    set "TARGET_DIR="
    for /r "%SCRIPT_DIR%\..\boards" %%j in (.) do (
        if /i "%%~nxj"=="!BOARD!" (
            set "TARGET_DIR=%%j"
            goto Found
        )
    )
    :Found
    if defined TARGET_DIR (
        echo !BOARD! support found at !TARGET_DIR!, getting board dependency...
        call "!TARGET_DIR!\deps\install.bat"
    ) else (
        echo !BOARD! support not found, skip...
    )
)

endlocal
