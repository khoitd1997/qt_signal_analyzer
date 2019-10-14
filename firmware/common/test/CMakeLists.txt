set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -lusb-1.0 -Wall")

add_executable(deserialize_test main.cpp)
target_link_libraries(deserialize_test PUBLIC signal_analyzer_common)
target_include_directories(deserialize_test PUBLIC /usr/include/libusb-1.0)
