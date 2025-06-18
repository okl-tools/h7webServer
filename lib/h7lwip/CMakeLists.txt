# lib/h7lwip/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)

# LwIP-only static library setup
project(lwip-lib LANGUAGES C)

set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}")

# LwIP include directories (core + port + config)
set(LWIP_Include_Dirs
    ${PROJECT_ROOT}/LwIP/src/include
    ${PROJECT_ROOT}/LwIP/system
    ${PROJECT_ROOT}/Core/Inc # for lwipopts.h
)

# LwIP source files (core stack + common modules)
file(GLOB_RECURSE LWIP_SOURCES
    ${PROJECT_ROOT}/LwIP/src/core/*.c
    ${PROJECT_ROOT}/LwIP/src/api/*.c
    ${PROJECT_ROOT}/LwIP/src/netif/*.c
)


# Remove optional/specialized files if needed (e.g., SNMP, PPP, etc.)
foreach(src IN LISTS LWIP_SOURCES)
    if(src MATCHES "/snmp/" OR src MATCHES "/ppp/" OR src MATCHES "/modbus/")
        list(REMOVE_ITEM LWIP_SOURCES ${src})
    endif()
endforeach()

list(APPEND LWIP_SOURCES
        ${PROJECT_ROOT}/LwIP/system/OS/sys_arch.c
)
list(APPEND LWIP_SOURCES
        ${PROJECT_ROOT}/LwIP/src/apps/http/fs.c
)


add_library(lwip STATIC ${LWIP_SOURCES})

target_include_directories(lwip PUBLIC
        ${PROJECT_ROOT}/LwIP/src/include
        ${PROJECT_ROOT}/LwIP/src/include/lwip/apps  # ← wichtig für fs.h
        ${PROJECT_ROOT}/LwIP/system          # ← das ist wichtig
)



# Am Ende von lib/h7lwip/CMakeLists.txt
target_link_libraries(lwip PUBLIC rtos)
