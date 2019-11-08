cmake_minimum_required(VERSION 3.14)
project(signal_analyzer_firmware C CXX ASM)

set(
  CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -std=c++17 -fno-rtti -Wno-comment -Og -Wall --specs=nosys.specs -fno-exceptions"
  )
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(cube_generated)
list(TRANSFORM
     STM32_HAL_SRCS
     PREPEND
     ${CMAKE_CURRENT_SOURCE_DIR}/cube_generated/)
list(TRANSFORM
     STM32_HAL_INCS
     PREPEND
     ${CMAKE_CURRENT_SOURCE_DIR}/cube_generated/)
include_directories(SYSTEM ${STM32_HAL_INCS})

link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/cmsis/dsp/libarm_cortexM4lf_math.a)
include_directories(SYSTEM cmsis/core/include)
include_directories(SYSTEM cmsis/dsp/include)

add_subdirectory(debug_utils)

add_subdirectory(common)

add_executable(${PROJECT_NAME}.elf ${STM32_HAL_SRCS} firmware_main.cpp)
target_link_libraries(${PROJECT_NAME}.elf PUBLIC signal_analyzer_common swo)
