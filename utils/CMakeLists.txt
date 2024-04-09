add_library(utils STATIC Angle.c)

target_include_directories(utils PUBLIC ${CMAKE_CURRENT_LIST_DIR})
