file(GLOB CORE_SOURCES *.c *.cpp)
add_library(core STATIC ${CORE_SOURCES})

target_link_libraries(core board_static)