MOC_ROOT := $(CURDIR)

BOARD ?= nrf52840dk


BOARD_DIR ?= $(shell cd ${MOC_ROOT} && find "./boards" -type d -name "${BOARD}" -print -quit)

BUILD_DIR ?= build

OUT_DIR ?= out

include $(MOC_ROOT)/boards/$(BOARD).mk

