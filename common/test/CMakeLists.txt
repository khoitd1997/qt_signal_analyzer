set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

add_executable(deserialize_test main.cpp)
target_link_libraries(deserialize_test PUBLIC signal_analyzer_common)
