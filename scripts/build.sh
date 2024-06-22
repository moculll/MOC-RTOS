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


CPU_MAX=$(cat /proc/cpuinfo | grep "processor" | wc -l)
cd "${MOCROOT_DIR}"
source "${BOARD_DIR}/deps/env.sh"

make clean

make "BOARD=${BOARD}" "-j${CPU_MAX}"



mkdir "${OUTPUT_DIR}"
mv "${MOCROOT_DIR}/${BOARD}.bin" "${MOCROOT_DIR}/${BOARD}.elf" "${MOCROOT_DIR}/${BOARD}.hex" "${MOCROOT_DIR}/${BOARD}.map" "${OUTPUT_DIR}"

