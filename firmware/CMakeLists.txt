cmake_minimum_required(VERSION 3.14)
project(signal_analyzer_firmware C CXX ASM)

set(
  CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -std=c++17 -fno-rtti -Wno-comment -Wall --specs=nosys.specs -fno-exceptions"
  )

add_subdirectory(cube_generated)

link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/cmsis/dsp/libarm_cortexM4lf_math.a)
include_directories(SYSTEM cmsis/core/include)
include_directories(SYSTEM cmsis/dsp/include)

list(TRANSFORM
     STM32_HAL_SRCS
     PREPEND
     ${CMAKE_CURRENT_SOURCE_DIR}/cube_generated/)
list(TRANSFORM
     STM32_HAL_INCS
     PREPEND
     ${CMAKE_CURRENT_SOURCE_DIR}/cube_generated/)
add_executable(${PROJECT_NAME}.elf ${STM32_HAL_SRCS} firmware_main.cpp)
target_include_directories(${PROJECT_NAME}.elf PUBLIC ${STM32_HAL_INCS})
