#!/bin/env bash
SCRIPT_DIR=$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")
BOARDS_ARRAY=$(grep 'BOARDS=' "${SCRIPT_DIR}/../config/boards.cfg")
SPILT=$(echo "$BOARDS_ARRAY" | sed -e 's/BOARDS=\[//' -e 's/\]//')
SPILT=$(echo "$SPILT" | tr -d ' ' | tr ',' '\n')
IFS=$'\n' read -rd '' -a BOARDS <<< "$SPILT"

for BOARD in "${BOARDS[@]}"; do
    TARGET_DIR=$(find "${SCRIPT_DIR}/../boards/" -type d -name "${BOARD}" -print -quit)
    if [ -n "${TARGET_DIR}" ]; then
        echo "${BOARD} support found, getting board dependency..."
        . "${TARGET_DIR}/deps/install.sh"
    else
        echo "${BOARD} support not found, skip..."
    fi
done

