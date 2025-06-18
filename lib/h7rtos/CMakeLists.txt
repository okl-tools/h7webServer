# lib/h7rtos/CMakeLists.txt

cmake_minimum_required(VERSION 3.22)


project(rtos-lib LANGUAGES C)

set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}")

add_compile_definitions(STM32H723xx)


set(RTOS_Include_Dirs
    ${PROJECT_ROOT}/FreeRTOS/Source/include
    ${PROJECT_ROOT}/FreeRTOS/Source/CMSIS_RTOS_V2
    ${PROJECT_ROOT}/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1
    ${PROJECT_ROOT}/Core/Inc
)

list(APPEND RTOS_Include_Dirs
        ${PROJECT_ROOT}/Drivers/CMSIS/Include
        ${PROJECT_ROOT}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
)


# Nur Kernel-Code (ohne portable)
file(GLOB RTOS_KERNEL_SOURCES
        ${PROJECT_ROOT}/FreeRTOS/Source/tasks.c
        ${PROJECT_ROOT}/FreeRTOS/Source/queue.c
        ${PROJECT_ROOT}/FreeRTOS/Source/list.c
        ${PROJECT_ROOT}/FreeRTOS/Source/timers.c
        ${PROJECT_ROOT}/FreeRTOS/Source/event_groups.c
        ${PROJECT_ROOT}/FreeRTOS/Source/stream_buffer.c
)

# CMSIS RTOS wrapper
file(GLOB RTOS_CMSIS_SOURCES
        ${PROJECT_ROOT}/FreeRTOS/Source/CMSIS_RTOS_V2/*.c
)

# Portierung nur für M7
file(GLOB RTOS_PORT_SOURCES
        ${PROJECT_ROOT}/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/*.c
)

# Heap-Implementierung (z. B. heap_4.c)
set(RTOS_HEAP ${PROJECT_ROOT}/FreeRTOS/Source/portable/MemMang/heap_4.c)

# Final zusammensetzen
set(RTOS_SOURCES
        ${RTOS_KERNEL_SOURCES}
        ${RTOS_CMSIS_SOURCES}
        ${RTOS_PORT_SOURCES}
        ${RTOS_HEAP}
)

# Und hänge dann explizit heap_4.c an
list(APPEND RTOS_SOURCES
        ${PROJECT_ROOT}/FreeRTOS/Source/portable/MemMang/heap_4.c
)
add_library(rtos STATIC ${RTOS_SOURCES})
target_include_directories(rtos PUBLIC ${RTOS_Include_Dirs})

