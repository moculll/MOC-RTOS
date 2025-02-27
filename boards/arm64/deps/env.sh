#!/bin/env bash

SCRIPT_DIR=$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")

export PATH="${SCRIPT_DIR}/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf/bin":"${PATH}"

export FLASH_CMD="nrfjprog -f nrf52 --sectorerase --program out/nrf52840dk/MOC-RTOS.bin --reset"