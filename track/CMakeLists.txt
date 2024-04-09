
add_library(track STATIC track )
target_include_directories(track PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(track PRIVATE ../common/)