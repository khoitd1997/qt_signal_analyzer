cmake_minimum_required(VERSION 3.14)
project(common)

add_library(signal_analyzer_common INTERFACE)
target_include_directories(signal_analyzer_common INTERFACE include)

if(NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "arm")
  add_subdirectory(test)
endif()
