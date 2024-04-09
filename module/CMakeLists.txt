add_library(module STATIC module.c)

target_include_directories(module PUBLIC ${CMAKE_CURRENT_LIST_DIR})
