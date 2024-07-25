#!/bin/env bash
if [ $# -lt 1 ]; then
    echo "Usage:"
    echo -e "\t$0 BOARD=nrf52840dk"
fi

SCRIPT_DIR=$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")

MOCROOT_DIR="${SCRIPT_DIR}/.."

BOARD=$(echo "${1}" | cut -d'=' -f2)

BOARD_DIR=$(find "${SCRIPT_DIR}/../boards/" -type d -name "${BOARD}" -print -quit)

TARGET_DIR=$(find "${SCRIPT_DIR}/../boards/" -type d -name "${BOARD}" -print -quit)

source "${TARGET_DIR}/deps/env.sh"

echo "[FLASH] flashing..."

${FLASH_CMD} > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "[FLASH] flash done."
else
    echo "[FLASH] flash failed, check!"
fi