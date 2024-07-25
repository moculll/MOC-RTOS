cmake_minimum_required(VERSION 3.10)
project(MOC-RTOS)

if(DEFINED BOARD)
    message("[BUILD] BOARD: ${BOARD}")
else()
    set(BOARD nrf52840dk CACHE STRING "board for compiling")
    message("[BUILD] BOARD is not defined, using default BOARD: nrf52840dk")
endif()




set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/boards/${BOARD}.cmake")



include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/lib)

include(${CMAKE_TOOLCHAIN_FILE})

add_subdirectory(core)
add_subdirectory(lib)
add_subdirectory(examples)
