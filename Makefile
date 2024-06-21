MOC_ROOT := $(CURDIR)

BOARD ?= nrf52840dk


BOARD_DIR := $(shell find "${MOC_ROOT}/boards" -type d -name "${BOARD}" -print -quit)

include $(MOC_ROOT)/boards/$(BOARD).mk




#include $(MOC_ROOT)/boards/$(BOARD).mk
