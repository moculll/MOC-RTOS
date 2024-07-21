file(GLOB CORE_SOURCES *.c)
add_library(core STATIC ${CORE_SOURCES})
target_link_libraries(core moc_rt)