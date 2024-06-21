MOC_ROOT = $(PWD)

TARGET ?= nrf52840dk


#include $(MOC_ROOT)/boards/$(TARGET).mk

BOARD_DIR := $(MOC_ROOT)/boards/arm/nrf52840dk



CC := arm-none-eabi-gcc
LD := arm-none-eabi-gcc
AS := arm-none-eabi-as
OBJCOPY := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size


VENDOR_LIB := $(MOC_ROOT)/vendor/nordic/nrfx
CFLAGS := -mcpu=cortex-m4 -mthumb -mabi=aapcs -mfloat-abi=hard -DFLOAT_ABI_HARD -mfpu=fpv4-sp-d16 -D__START=main -D__Vectors=__isr_vector -D__STARTUP_CLEAR_BSS

CFLAGS += -DNRF52840_XXAA -DCONFIG_GPIO_AS_PINRESET -DCONFIG_NFCT_PINS_AS_GPIOS
CFLAGS += -lc -lnosys -lm



SRCS := $(wildcard $(VENDOR_LIB)/drivers/src/*.c)
SRCS += $(wildcard $(VENDOR_LIB)/drivers/soc/*.c)
SRCS += $(wildcard $(VENDOR_LIB)/drivers/src/prs/*.c)
SRCS += $(MOC_ROOT)/examples/hello_world/src/main.c

SRCS += $(wildcard $(MOC_ROOT)/lib/shellMgr/*.c)
SRCS += $(wildcard $(MOC_ROOT)/boards/arm/nrf52840dk/osImpl/*.c)


SRCS_ASM := $(wildcard $(MOC_ROOT)/boards/arm/nrf52840dk/reset_handler/*.S)

SRCS += $(wildcard $(MOC_ROOT)/boards/arm/nrf52840dk/reset_handler/*.c)

CFLAGS += -I$(VENDOR_LIB) -I$(VENDOR_LIB)/drivers -I$(VENDOR_LIB)/drivers/include 
CFLAGS += -I$(VENDOR_LIB)/drivers/src -I$(VENDOR_LIB)/drivers/src/prs -I$(VENDOR_LIB)/hal -I$(VENDOR_LIB)/soc
CFLAGS += -I$(MOC_ROOT)/boards/arm/nrf52840dk -I$(MOC_ROOT)/boards/arm/nrf52840dk/reset_handler

CFLAGS += -I$(MOC_ROOT)/lib/cmsis/Core/Include -I$(MOC_ROOT)/examples/hello_world/src

CFLAGS += -I$(MOC_ROOT)/lib/shellMgr -I$(MOC_ROOT)/boards/arm/nrf52840dk/osImpl



LDSCRIPT := $(BOARD_DIR)/ld/nrf_common.ld
LDFLAGS := -T $(LDSCRIPT) -Wl,-Map,main.map --specs=nano.specs -Wl,--gc-sections




OBJS := $(SRCS:.c=.o)
OBJS += $(SRCS_ASM:.S=.o)

TARGET := main
all: $(TARGET).elf



%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET).elf: $(OBJS)
	$(LD) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@
	$(OBJCOPY) -O ihex $(TARGET).elf $(TARGET).hex
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin
	$(SIZE) $(TARGET).elf

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).hex $(TARGET).bin main.map

.PHONY: all clean