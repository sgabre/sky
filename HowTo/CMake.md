Software Component
* configuration
* interfaces
* sources

Depository
* inc/SoftwareComponent/SoftwareComponent_Config.h
* inc/SoftwareComponent/SoftwareComponent_API.h
* lib/SoftwareComponent/SoftwareComponent.lib
* lib/cmake/SoftwareComponent/*.cmake



#Build a Project
cmake --build <dir> [<options>] [-- <build-tool-options>]
	
# Install a Project
cmake --install <dir> [<options>]
		
# Run a Script
cmake [-D <var>=<value>]... -P <cmake-script-file>
				
# Run a Command-Line Tool
cmake -E <command> [<options>]
					
# Run the Find-Package Tool
cmake --find-package [<options>]
						
# Run a Workflow Preset
cmake --workflow [<options>]
							
						

# How to build 

# How to test 

# How to install

cmake --install .

cmake --build . --target install --config Debug

cmake --install . --prefix "/Users/sgabre/git/depository"


# How to pack 

    
## Message
# specify the C standard
#set(CMAKE_C_STANDARD 11)
#set(CMAKE_C_STANDARD_REQUIRED True)


message("  ")

message("SOFWARE_COMPONENT_NAME: " ${SOFWARE_COMPONENT_NAME})
message("  ")

#message("CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER})


#message("CMAKE_TOOLCHAIN_FILE: " ${CMAKE_TOOLCHAIN_FILE})

#message("CMAKE_AR: " ${CMAKE_AR})
#message("CMAKE_ASM_COMPILER: " ${CMAKE_ASM_COMPILER})
#message("CMAKE_LINKER: " ${CMAKE_LINKER})
#message("CMAKE_OBJCOPY: " ${CMAKE_OBJCOPY})
#message("CMAKE_RANLIB: " ${CMAKE_RANLIB})



#message("CMAKE_HOST_SYSTEM_NAME: " ${CMAKE_HOST_SYSTEM_NAME})
#message("CMAKE_SYSTEM_NAME: " ${CMAKE_SYSTEM_NAME})

#message("BUILD_COMPONENT_TEST: " ${BUILD_COMPONENT_TEST})
#message(" BUILD_COMPONENT_CLI: " ${BUILD_COMPONENT_CLI})


#add_library(${SOFWARE_COMPONENT_NAME} SHARED)
#target_include_directories(${SOFWARE_COMPONENT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}")
#add_subdirectory("configuration")
#add_subdirectory("interfaces")
#add_subdirectory("sources")


