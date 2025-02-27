cmake_minimum_required(VERSION 3.10)
enable_language(ASM)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
#set(CROSS_TOOL ${CMAKE_SOURCE_DIR}/boards/arm/nrf52840dk/deps/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux/bin)

set(CMAKE_C_COMPILER aarch64-none-elf-gcc)
set(CMAKE_CXX_COMPILER aarch64-none-elf-g++)
set(CMAKE_ASM_COMPILER aarch64-none-elf-gcc)
set(CMAKE_OBJCOPY aarch64-none-elf-objcopy)
set(CMAKE_OBJDUMP aarch64-none-elf-objdump)
set(CMAKE_SIZE aarch64-none-elf-size)
set(CMAKE_LINKER aarch64-none-elf-ld)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb -mtune=cortex-a53 -lc -lnosys -lm -nostartfiles -ffreestanding --specs=nosys.specs")
add_definitions(-D__STARTUP_CLEAR_BSS -D__START=main -D__Vectors=__isr_vector)
set(LDSCRIPT ${CMAKE_SOURCE_DIR}/boards/arm64/virt/ld/qemu-virt-aarch64.ld)
set(CMAKE_EXE_LINKER_FLAGS "-L${CMAKE_SOURCE_DIR}/boards/arm64/linker_scripts -T${LDSCRIPT}")

include_directories(
    ${CMAKE_SOURCE_DIR}/boards/arm64
)


file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/boards/arm64/osImpl/*.c
    ${CMAKE_SOURCE_DIR}/boards/arm64/virt/DebugTrace/*.cpp
    ${CMAKE_SOURCE_DIR}/boards/arm64/virt/reset_handler/*.c
    ${CMAKE_SOURCE_DIR}/boards/arm64/virt/reset_handler/start.S

)

add_library(board_static STATIC ${SOURCES})


target_link_libraries(board_static core shellMgr drivers)


