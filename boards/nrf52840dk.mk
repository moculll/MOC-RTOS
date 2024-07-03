
CC := arm-none-eabi-gcc
LD := arm-none-eabi-gcc
AS := arm-none-eabi-as
OBJCOPY := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size


VENDOR_LIB := ./vendor/nordic/nrfx


# CPU CFLAGS
CFLAGS := -mcpu=cortex-m4 -mthumb -mabi=aapcs -mfloat-abi=hard -DFLOAT_ABI_HARD -mfpu=fpv4-sp-d16
CFLAGS += -lc -lnosys -lm -nostartfiles

# Vendor Chip CFLAGS
CFLAGS += -DNRF52840_XXAA -DCONFIG_GPIO_AS_PINRESET -DCONFIG_NFCT_PINS_AS_GPIOS -D__STARTUP_CLEAR_BSS

# MOC-KERNEL SYSTEM CFLAGS
CFLAGS += -D__START=mocStartEntry -D__Vectors=__isr_vector


# nrfx lib
SRCS := $(wildcard $(VENDOR_LIB)/drivers/src/*.c)
SRCS += $(wildcard $(VENDOR_LIB)/drivers/soc/*.c)
SRCS += $(wildcard $(VENDOR_LIB)/drivers/src/prs/*.c)
SRCS += $(wildcard $(BOARD_DIR)/reset_handler/*.c)
CFLAGS += -I$(VENDOR_LIB) -I$(VENDOR_LIB)/drivers -I$(VENDOR_LIB)/drivers/include 
CFLAGS += -I$(VENDOR_LIB)/drivers/src -I$(VENDOR_LIB)/drivers/src/prs -I$(VENDOR_LIB)/hal -I$(VENDOR_LIB)/soc
CFLAGS += -I$(BOARD_DIR)/vendor


# CMSIS library include, better include this library directly, other libs wouldn't modify
CFLAGS += -I./lib/cmsis/Core/Include

# mocos System lib include(dlist/cmsis/shellMgr)
CFLAGS += -I./lib

# mocos shell namager src
SRCS += $(wildcard ./lib/shellMgr/*.c)

# mocos system core src
SRCS += $(wildcard ./core/*.c)

# mocos system include
CFLAGS += -I./include

# mocos system implementation
SRCS += $(wildcard $(BOARD_DIR)/osImpl/*.c)
CFLAGS += -I$(BOARD_DIR)


# mocos system board memory map implementation
LDSCRIPT := $(BOARD_DIR)/ld/nrf_common.ld
LDFLAGS := -T $(LDSCRIPT) -Wl,-Map,$(BUILD_DIR)/$(OUT_DIR)/$(BOARD).map --specs=nano.specs -Wl,--gc-sections

# example src
SRCS += ./examples/hello_world/src/main.c

SRCS_ASM := $(wildcard ./boards/arm/nrf52840dk/reset_handler/*.S)


OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))
OBJS += $(patsubst %.S, $(BUILD_DIR)/%.o, $(SRCS_ASM))


all: $(BUILD_DIR)/$(OUT_DIR)/$(BOARD).elf



$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/$(OUT_DIR)/$(BOARD).elf: $(OBJS)
	@mkdir -p $(BUILD_DIR)/$(OUT_DIR)
	$(LD) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@
	$(OBJCOPY) -O ihex $(BUILD_DIR)/$(OUT_DIR)/$(BOARD).elf $(BUILD_DIR)/$(OUT_DIR)/$(BOARD).hex
	$(OBJCOPY) -O binary $(BUILD_DIR)/$(OUT_DIR)/$(BOARD).elf $(BUILD_DIR)/$(OUT_DIR)/$(BOARD).bin
	$(SIZE) $(BUILD_DIR)/$(OUT_DIR)/$(BOARD).elf

clean:
	rm -rf ./build


.PHONY: all clean