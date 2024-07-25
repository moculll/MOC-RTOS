set(SYSCALLS_PATH ${CMAKE_SOURCE_DIR}/lib/newlib/syscalls.c)


target_sources(${PROJECT_NAME} PRIVATE ${SYSCALLS_PATH})
target_link_libraries(${PROJECT_NAME} shellMgr)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}>
    COMMENT "Creating hex and bin files"
)


add_custom_target(clean_build
    COMMAND rm -rf ${CMAKE_SOURCE_DIR}/build_bak
    COMMAND mv ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/build_bak
    COMMENT "Cleaning build directory"
)