cmake_minimum_required(VERSION 3.10)
enable_language(ASM)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
#set(CROSS_TOOL ${CMAKE_SOURCE_DIR}/boards/arm/nrf52840dk/deps/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux/bin)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE arm-none-eabi-size)

set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb -mabi=aapcs -mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lc -lnosys -lm -nostartfiles")
add_definitions(-DFLOAT_ABI_HARD -DNRF52840_XXAA -DCONFIG_GPIO_AS_PINRESET -D__STARTUP_CLEAR_BSS -DCONFIG_NFCT_PINS_AS_GPIOS -D__START=mocStartEntry -D__Vectors=__isr_vector)
set(LDSCRIPT ${CMAKE_SOURCE_DIR}/boards/arm/nrf52840dk/ld/nrf_common.ld)
set(CMAKE_EXE_LINKER_FLAGS "-T ${LDSCRIPT} -Wl,-Map,${CMAKE_BINARY_DIR}/nrf52840dk.map -Wl,--gc-sections")

include_directories(
    ${CMAKE_SOURCE_DIR}/boards/arm/nrf52840dk
    ${CMAKE_SOURCE_DIR}/boards/arm/nrf52840dk/vendor
    ${CMAKE_SOURCE_DIR}/lib/cmsis/Core/Include
    ${CMAKE_SOURCE_DIR}/vendor/nordic/nrfx
    ${CMAKE_SOURCE_DIR}/vendor/nordic/nrfx/drivers
    ${CMAKE_SOURCE_DIR}/vendor/nordic/nrfx/drivers/include
    ${CMAKE_SOURCE_DIR}/vendor/nordic/nrfx/drivers/src
    ${CMAKE_SOURCE_DIR}/vendor/nordic/nrfx/drivers/src/prs
    ${CMAKE_SOURCE_DIR}/vendor/nordic/nrfx/hal
    ${CMAKE_SOURCE_DIR}/vendor/nordic/nrfx/soc
)

file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/boards/arm/nrf52840dk/osImpl/*.c
    ${CMAKE_SOURCE_DIR}/boards/arm/nrf52840dk/reset_handler/*.c
    ${CMAKE_SOURCE_DIR}/boards/arm/nrf52840dk/reset_handler/gcc_startup_nrf52840.S
    ${CMAKE_SOURCE_DIR}/vendor/nordic/nrfx/drivers/src/*.c
    ${CMAKE_SOURCE_DIR}/vendor/nordic/nrfx/soc/*.c
)

add_library(board_static STATIC ${SOURCES})

target_link_libraries(board_static core shellMgr)


