## CMakeLists @ SoftwareComponent Directory 
cmake_minimum_required(VERSION 3.23)
set(SOFWARE_COMPONENT_NAME SoftwareComponent)
project(${SOFWARE_COMPONENT_NAME} LANGUAGES C VERSION 5.4.3)

#Options
option(BUILD_COMPONENT_TEST "Build Component Testing" OFF)
option(BUILD_COMPONENT_CLI "Build Command-In-Line Interfaces for the LSTime Software Component" ON)

if(BUILD_COMPONENT_TEST STREQUAL "ON")
	set(BUILD_COMPONENT_CLI ON)
endif()

if (MSVC)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)
endif()

#add_library(${SOFWARE_COMPONENT_NAME} SHARED sources/SoftwareComponent.c sources/SoftwareComponent.h)
add_library(${SOFWARE_COMPONENT_NAME} SHARED)
#target_include_directories(${SOFWARE_COMPONENT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}")

target_include_directories(${SOFWARE_COMPONENT_NAME}
														INTERFACE
														$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
														$<INSTALL_INTERFACE:inc>
														)

#add_subdirectory("configuration")
add_subdirectory("interfaces")
add_subdirectory("sources")


# Command-In-Line Interfaces
if(BUILD_COMPONENT_CLI STREQUAL "ON")
	add_executable(${SOFWARE_COMPONENT_NAME}Shell sources/main.c )
	target_link_libraries(${SOFWARE_COMPONENT_NAME}Shell PUBLIC ${SOFWARE_COMPONENT_NAME})
endif()



#Test
if(BUILD_COMPONENT_TEST STREQUAL "ON")
	message("Build Component Testing...")
	enable_testing()
	function(ComponentTest target arg result)
		add_test(NAME "ComponentTest ${arg}" COMMAND ${target} ${arg})
		set_tests_properties("ComponentTest ${arg}" PROPERTIES PASS_REGULAR_EXPRESSION ${result})
	endfunction()
	
	#CLI ./SoftwareComponentShell --version  			Expected: Local Sidereal Time: 2.74046 @ LNG: 1 @ 01/01/1968 00:00:00
	#ComponentTest(LSTime --lng 0 "Local Sidereal Time: 2.74046 @ LNG: 0 @ 01/01/1968 00:00:00")
	
endif()

#Install 
install(TARGETS ${SOFWARE_COMPONENT_NAME} 
	EXPORT T${SOFWARE_COMPONENT_NAME}Targets
	FILE_SET HEADERS
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION inc)

set(INCLUDE_INSTALL_DIR inc/)

#	FILE ${SOFWARE_COMPONENT_NAME}Targets.cmake
#	DESTINATION lib/cmake/${SOFWARE_COMPONENT_NAME})

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
	"${SOFWARE_COMPONENT_NAME}Version.cmake"
	VERSION ${SOFTWARE_COMPONENT_VERSION}
	COMPATIBILITY AnyNewerVersion)

install(FILES interfaces/${SOFWARE_COMPONENT_NAME}_API.h DESTINATION inc/${SOFWARE_COMPONENT_NAME})
install(FILES configuration/${SOFWARE_COMPONENT_NAME}_Config.h DESTINATION inc/${SOFWARE_COMPONENT_NAME})

install(EXPORT ${SOFWARE_COMPONENT_NAME}Targets
    FILE ${SOFWARE_COMPONENT_NAME}Targets.cmake
    DESTINATION lib/cmake/${SOFWARE_COMPONENT_NAME})

install(FILES "${SOFWARE_COMPONENT_NAME}Config.cmake" "${CMAKE_CURRENT_BINARY_DIR}/${SOFWARE_COMPONENT_NAME}Version.cmake"
	DESTINATION lib/cmake/${SOFWARE_COMPONENT_NAME})




#mkdir longtitleproject && cd $_
#cmake ..
#make
#cmake --install . --prefix ../../geo-install
#cmake --install . --config Debug --prefix "/Users/sgabre/git/install"





