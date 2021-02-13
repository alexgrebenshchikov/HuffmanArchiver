cmake_minimum_required(VERSION 3.10)
project(hw_3)
include_directories(include)
set(CMAKE_CXX_STANDARD 17)

add_executable(hw_03 src/main.cpp src/huffman.cpp include/huffman.h )
add_executable(hw_03_test test/test.cpp test/autotest.cpp test/autotest.h test/huffman_test.cpp test/huffman_test.h src/huffman.cpp include/huffman.h)