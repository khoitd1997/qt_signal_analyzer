cmake_minimum_required(VERSION 3.14)
project(common)

add_library(signal_analyzer_common INTERFACE)
target_include_directories(signal_analyzer_common INTERFACE .)
