

file(GLOB MMEMORYMGR_SOURCES "mMemoryMgr/*.c")
file(GLOB RBTREE_SOURCES "rbtree/*.c")
file(GLOB SHELLMGR_SOURCES "shellMgr/*.c")

add_library(mMemoryMgr STATIC ${MMEMORYMGR_SOURCES})
add_library(shellMgr STATIC ${SHELLMGR_SOURCES})
add_library(rbtree STATIC ${RBTREE_SOURCES})
target_link_libraries(mMemoryMgr rbtree)
target_link_libraries(shellMgr board_static)


include_directories(${CMAKE_SOURCE_DIR}/lib/cmsis/Core/Include)
include_directories(${CMAKE_SOURCE_DIR}/lib/dlist)

include_directories(rbtree PUBLIC ${CMAKE_SOURCE_DIR}/lib/rbtree)
include_directories(mMemoryMgr PUBLIC ${CMAKE_SOURCE_DIR}/lib/mMemoryMgr)
include_directories(mMemoryMgr PUBLIC ${CMAKE_SOURCE_DIR}/lib/rbtree)
include_directories(shellMgr PUBLIC ${CMAKE_SOURCE_DIR}/lib/shellMgr)




