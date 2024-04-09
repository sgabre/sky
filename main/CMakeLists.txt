
add_executable(main main.c)
target_link_libraries(main gauss intercept kepler lstime orbit site shell track utils)
target_include_directories(main PRIVATE ../common/)