cmake_minimum_required(VERSION 3.13)
project(glpkxx)

include_directories(./glpk-4.65/src/)
link_directories(/usr/local/lib)
link_libraries(glpk)

set(CMAKE_CXX_STANDARD 14)

add_library(glpkxx glpkxx.cpp glpkxx.h)

add_executable(glpkxx-test test.cpp glpkxx.h)
target_link_libraries(glpkxx-test glpkxx)