cmake_minimum_required(VERSION 3.10)
project(MOC-RTOS)

set(BOARD nrf52840dk CACHE STRING "board for compiling")

if (BOARD STREQUAL "nrf52840dk")
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/boards/nrf52840dk.cmake")
endif()


include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/lib)

include(${CMAKE_TOOLCHAIN_FILE})

add_subdirectory(core)
add_subdirectory(lib)
add_subdirectory(examples)
