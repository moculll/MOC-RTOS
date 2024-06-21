#!/bin/env bash
if [ $# -ne 1 ]; then
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
#FIXME: haven't done clean function
#make "BOARD=${BOARD} clean"

make "BOARD=${BOARD}"



mkdir "${OUTPUT_DIR}"
mv "${MOCROOT_DIR}/${BOARD}.bin" "${MOCROOT_DIR}/${BOARD}.elf" "${MOCROOT_DIR}/${BOARD}.hex" "${MOCROOT_DIR}/${BOARD}.map" "${OUTPUT_DIR}"

