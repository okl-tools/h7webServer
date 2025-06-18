cmake_minimum_required(VERSION 3.22)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# STM32CubeMX generated symbols (macros)
set(MX_Defines_Syms
        USE_PWR_LDO_SUPPLY
        USE_NUCLEO_64
        USE_HAL_DRIVER
        STM32H723xx
        $<$<CONFIG:Debug>:DEBUG>
)

# Base path to project root (assumes this file is in lib/h7hal/)
#get_filename_component(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}")

# STM32CubeMX generated include paths
set(MX_Include_Dirs
        ${PROJECT_ROOT}/Core/Inc
        ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Inc
        ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
        ${PROJECT_ROOT}/Drivers/BSP/STM32H7xx_Nucleo
        ${PROJECT_ROOT}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
        ${PROJECT_ROOT}/Drivers/CMSIS/Include
)

# Gather all HAL driver and BSP source files using wildcards
file(GLOB_RECURSE H7_ALL_SOURCES
        ${PROJECT_ROOT}/Drivers/STM32H7xx_HAL_Driver/Src/*.c
        ${PROJECT_ROOT}/Drivers/BSP/STM32H7xx_Nucleo/*.c
        ${PROJECT_ROOT}/Drivers/BSP/Components/lan8742/*.c
        ${PROJECT_ROOT}/Core/Src/system_stm32h7xx.c
)

# Remove all *_template.c files automatically
foreach(src IN LISTS H7_ALL_SOURCES)
    if(src MATCHES ".*_template\\.c$")
        list(REMOVE_ITEM H7_ALL_SOURCES ${src})
    endif()
endforeach()



# Create interface for includes and defines
add_library(h7_headers INTERFACE)
target_include_directories(h7_headers INTERFACE ${MX_Include_Dirs})
target_compile_definitions(h7_headers INTERFACE ${MX_Defines_Syms})

# Create the static library with all HAL components
add_library(h7-all STATIC ${H7_ALL_SOURCES})
target_link_libraries(h7-all PUBLIC h7_headers)

# Validate that STM32CubeMX code is compatible with C standard
if((CMAKE_C_STANDARD EQUAL 90) OR (CMAKE_C_STANDARD EQUAL 99))
    message(FATAL_ERROR "Generated code requires C11 or higher")
endif()
