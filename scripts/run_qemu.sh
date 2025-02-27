#!/bin/bash

SCRIPT_DIR=$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")
qemu-system-aarch64 -semihosting -m 128M -nographic -monitor none -serial stdio  -machine virt,gic-version=2,secure=on,virtualization=on -cpu cortex-a53 -smp 2 -kernel ${SCRIPT_DIR}/../out/virt/MOC-RTOS.elf

