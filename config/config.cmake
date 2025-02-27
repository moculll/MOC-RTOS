set(SYSCALLS_PATH ${CMAKE_SOURCE_DIR}/lib/newlib/syscalls.c)


target_sources(${PROJECT_NAME}.elf PRIVATE ${SYSCALLS_PATH})
target_link_libraries(${PROJECT_NAME}.elf shellMgr)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -x -l -S ${PROJECT_NAME}.elf > ${PROJECT_NAME}.lst
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_SIZE} -t ${PROJECT_NAME}.elf
    COMMENT "Creating hex and bin files"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)


add_custom_target(clean_build
    COMMAND rm -rf ${CMAKE_SOURCE_DIR}/build_bak
    COMMAND mv ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/build_bak
    COMMENT "Cleaning build directory"
)