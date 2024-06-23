
CC := arm-none-eabi-gcc
LD := arm-none-eabi-gcc
AS := arm-none-eabi-as
OBJCOPY := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size


VENDOR_LIB := $(MOC_ROOT)/vendor/nordic/nrfx

CFLAGS := -mcpu=cortex-m4 -mthumb -mabi=aapcs -mfloat-abi=hard -DFLOAT_ABI_HARD -mfpu=fpv4-sp-d16
CFLAGS += -lc -lnosys -lm -nostartfiles

CFLAGS += -DNRF52840_XXAA -DCONFIG_GPIO_AS_PINRESET -DCONFIG_NFCT_PINS_AS_GPIOS -D__START=main -D__Vectors=__isr_vector -D__STARTUP_CLEAR_BSS




SRCS := $(wildcard $(VENDOR_LIB)/drivers/src/*.c)
SRCS += $(wildcard $(VENDOR_LIB)/drivers/soc/*.c)
SRCS += $(wildcard $(VENDOR_LIB)/drivers/src/prs/*.c)
SRCS += $(wildcard $(BOARD_DIR)/reset_handler/*.c)

# mocos shell namager src
SRCS += $(wildcard $(MOC_ROOT)/lib/shellMgr/*.c)

# mocos system core src
SRCS += $(wildcard $(MOC_ROOT)/core/*.c)

# nrf52840dk system implementation src
SRCS += $(wildcard $(BOARD_DIR)/osImpl/*.c)

# example src
SRCS += $(MOC_ROOT)/examples/hello_world/src/main.c

SRCS_ASM := $(wildcard $(MOC_ROOT)/boards/arm/nrf52840dk/reset_handler/*.S)


# FIXME: this is not the correct way to include files in nrfx
CFLAGS += -I$(VENDOR_LIB) -I$(VENDOR_LIB)/drivers -I$(VENDOR_LIB)/drivers/include 
CFLAGS += -I$(VENDOR_LIB)/drivers/src -I$(VENDOR_LIB)/drivers/src/prs -I$(VENDOR_LIB)/hal -I$(VENDOR_LIB)/soc
CFLAGS += -I$(BOARD_DIR) -I$(BOARD_DIR)/vendor -I$(BOARD_DIR)/reset_handler

# CMSIS library include
CFLAGS += -I$(MOC_ROOT)/lib/cmsis/Core/Include

# mocos system core include
CFLAGS += -I$(MOC_ROOT)/core/include

# mocos shell manager include
CFLAGS += -I$(MOC_ROOT)/lib/shellMgr 

# nrf52840dk system implementation include
CFLAGS += -I$(BOARD_DIR)/osImpl

# zephyr dlist
CFLAGS += -I$(MOC_ROOT)/lib/dlist

# example include
CFLAGS += -I$(MOC_ROOT)/examples/hello_world/src

LDSCRIPT := $(BOARD_DIR)/ld/nrf_common.ld

LDFLAGS := -T $(LDSCRIPT) -Wl,-Map,$(BOARD).map --specs=nano.specs -Wl,--gc-sections




OBJS := $(SRCS:.c=.o)
OBJS += $(SRCS_ASM:.S=.o)

all: $(BOARD).elf



%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

$(BOARD).elf: $(OBJS)
	$(LD) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@
	$(OBJCOPY) -O ihex $(BOARD).elf $(BOARD).hex
	$(OBJCOPY) -O binary $(BOARD).elf $(BOARD).bin
	$(SIZE) $(BOARD).elf

clean:
	rm -f $(OBJS) $(BOARD).elf $(BOARD).hex $(BOARD).bin $(BOARD).map

.PHONY: all clean