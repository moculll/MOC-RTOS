#!/bin/env bash

SCRIPT_DIR=$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")
URL=$(sed -n '1p' "${SCRIPT_DIR}/tools.dep")
DIR_NAME=$(basename "${URL}")
#echo "${SCRIPT_DIR}/${DIR_NAME}/bin"

export PATH="${SCRIPT_DIR}/${DIR_NAME}/bin":"${PATH}"

export FLASH_CMD="nrfjprog -f nrf52 --sectorerase --program out/nrf52840dk/MOC-RTOS.bin --reset"