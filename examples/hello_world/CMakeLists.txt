file(GLOB EXAMPLE_SOURCES src/*.c)

add_executable(${PROJECT_NAME} ${EXAMPLE_SOURCES})
target_link_libraries(${PROJECT_NAME} mMemoryMgr)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib)



# this overrides some of the syscalls in libc, functions like printf depend on it
include(${CMAKE_SOURCE_DIR}/config/config.cmake)