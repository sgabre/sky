
add_library(gauss STATIC gauss.c )
target_include_directories(gauss PUBLIC ${CMAKE_CURRENT_LIST_DIR})
