file(GLOB EXAMPLE_SOURCES src/*.c)

add_executable(hello_world ${EXAMPLE_SOURCES} ${CMAKE_SOURCE_DIR}/lib/newlib/syscalls.c)

target_link_libraries(hello_world shellMgr mMemoryMgr)

target_include_directories(hello_world PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(hello_world PRIVATE ${CMAKE_SOURCE_DIR}/lib)
add_custom_command(TARGET hello_world POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:hello_world> ${CMAKE_BINARY_DIR}/hello_world.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:hello_world> ${CMAKE_BINARY_DIR}/hello_world.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:hello_world>
    COMMENT "Creating hex and bin files"
)


add_custom_target(clean_build
    COMMAND rm -rf ${CMAKE_SOURCE_DIR}/build_bak
    COMMAND mv ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/build_bak
    COMMENT "Cleaning build directory"
)

