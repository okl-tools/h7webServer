cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()


# arm toolchain
include("cmake/gcc-arm-none-eabi.cmake")

# USE_FULL_ASSERT
add_compile_definitions(USE_FULL_ASSERT=1)

add_compile_definitions(H7_FREE_RTOS)

# information for IDEs like clion
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# project
set(CMAKE_PROJECT_NAME h7webServer)
project(${CMAKE_PROJECT_NAME} LANGUAGES C CXX ASM)

# HAL, RTOS, lwIP as Interface libraries
add_subdirectory(lib/h7hal)
add_subdirectory(lib/h7rtos)
add_subdirectory(lib/h7lwip)

# source
file(GLOB_RECURSE APP_SOURCES Core/Src/*.c)
file(GLOB_RECURSE APP_SOURCES_CPP Core/Src/*.cpp)
file(GLOB_RECURSE PTOOLS_SOURCES_CPP ptools/src/*.cpp)
list(REMOVE_ITEM APP_SOURCES ${CMAKE_SOURCE_DIR}/Core/Src/fsdata_custom.c)

# linker script
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32H723ZGTX_FLASH.ld)
set(CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT} -Wl,-Map=${CMAKE_PROJECT_NAME}.map")

# executable
add_executable(${CMAKE_PROJECT_NAME}
        ${APP_SOURCES}
        ${APP_SOURCES_CPP}
        ${PTOOLS_SOURCES_CPP}
        startup_stm32h723zgtx.s
)

# include
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE Core/Inc ptools/inc)

# lib
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE h7-all rtos lwip)

# clean
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_PROJECT_NAME}.map)