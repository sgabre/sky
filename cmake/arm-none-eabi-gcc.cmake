set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
set(CMAKE_C_COMPILER_ID GNU)
set(CMAKE_CXX_COMPILER_ID GNU)

# Some default GCC settings
# arm-none-eabi- must be part of path environment
set(TOOLCHAIN_PREFIX                arm-none-eabi-)

set(CMAKE_AR                        ${TOOLCHAIN_PREFIX}ar)
set(CMAKE_C_COMPILER                ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER              ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER              ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_LINKER                    ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_OBJCOPY                   ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE                      ${TOOLCHAIN_PREFIX}size)

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

# Without that flag CMake is not able to pass test compilation check
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# MCU specific flags
set(MCU_SETTINGS_FLAGS "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb")

# MCU Assembler flags
set(MCU_ASSEMBLER_WARNINGS_FLAGS  "-Wa, -W")
set(MCU_ASSEMBLER_DEBUG_LEVEL_FLAGS  "-g3")

set(CMAKE_ASM_FLAGS "${MCU_ASSEMBLER_WARNINGS_FLAGS} ${MCU_ASSEMBLER_DEBUG_LEVEL_FLAGS}" )

# MCU Compiler
set(MCU_COMPILER_GENERAL_FLAGS  "-std=gnu11")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(MCU_COMPILER_DEBUGING_FLAGS  "-g3")
    set(MCU_COMPILER_OPTIMIZATION_LEVEL_FLAGS  "-O0")
endif()
if(CMAKE_BUILD_TYPE MATCHES Release)
    set(MCU_COMPILER_DEBUGING_FLAGS  "-g0")
    set(MCU_COMPILER_OPTIMIZATION_LEVEL_FLAGS  "-Os")
endif()

set(MCU_COMPILER_OPTIMIZATIONS_FLAGS  "-fdata-sections -ffunction-sections")

set(MCU_COMPILER_MISCELLANEOUS_FLAGS  "-fstack-usage -fcyclomatic-complexity")

set(CMAKE_C_FLAGS "${MCU_COMPILER_GENERAL_FLAGS} ${MCU_COMPILER_DEBUGING_FLAGS} ${MCU_COMPILER_OPTIMIZATION_LEVEL_FLAGS}  ${MCU_COMPILER_OPTIMIZATIONS_FLAGS} ${MCU_COMPILER_MISCELLANEOUS_FLAGS}" )

# MCU Linker
set(MCU_LINKER_SCRIPT           "-T \"${CMAKE_SOURCE_DIR}/STM32F401RETx_FLASH.ld\"")
set(MCU_LINKER_SYSTEM_CALL      "--specs=nosys.specs")
set(MCU_LINKER_MAP_FILE         "-Wl,-Map=${CMAKE_PROJECT_NAME}.map")
set(MCU_LINKER_UNUSED_SECTION   "-Wl,--gc-sections")
set(MCU_LINKER_MATH_LIB         "-Wl,--start-group -lc -lm -Wl,--end-group")
#set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--print-memory-usage")
set(CMAKE_C_LINK_FLAGS "${MCU_LINKER_SCRIPT} ${MCU_LINKER_SYSTEM_CALL} ${MCU_LINKER_MAP_FILE} ${MCU_LINKER_UNUSED_SECTION} ${MCU_LINKER_MATH_LIB}" )
