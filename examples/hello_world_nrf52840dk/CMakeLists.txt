file(GLOB EXAMPLE_SOURCES src/*.c)

add_executable(${PROJECT_NAME}.elf ${EXAMPLE_SOURCES})
set_target_properties(${PROJECT_NAME}.elf PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
target_link_libraries(${PROJECT_NAME}.elf mMemoryMgr)

target_include_directories(${PROJECT_NAME}.elf PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${CMAKE_SOURCE_DIR}/lib)



# this overrides some of the syscalls in libc, functions like printf depend on it
include(${CMAKE_SOURCE_DIR}/config/config.cmake)