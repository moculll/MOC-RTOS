#!/bin/env bash
if [ $# -lt 1 ]; then
    echo "Usage:"
    echo -e "\t$0 BOARD=nrf52840dk"
fi
SCRIPT_DIR=$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")

MOCROOT_DIR="${SCRIPT_DIR}/.."

BOARD=$(echo "${1}" | cut -d'=' -f2)

BOARD_DIR=$(find "${SCRIPT_DIR}/../boards/" -type d -name "${BOARD}" -print -quit)

OUTPUT_DIR="${MOCROOT_DIR}/out/${BOARD}"


cd "${MOCROOT_DIR}"
source "${BOARD_DIR}/deps/env.sh"

cmake --build build --target clean_build > /dev/null
echo "[CLEAN] Moved previous build dir"

cmake -S . -G Ninja -B build "-DBOARD=${BOARD}" > /dev/null

cmake --build build

mkdir -p "${OUTPUT_DIR}"
cp "${MOCROOT_DIR}/build/MOC-RTOS.hex" "${MOCROOT_DIR}/build/${BOARD}.map" "${MOCROOT_DIR}/build/MOC-RTOS.bin" "${OUTPUT_DIR}/"
