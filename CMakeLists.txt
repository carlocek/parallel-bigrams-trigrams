cmake_minimum_required(VERSION 3.3)
project(parallel-bigrams-trigrams)             

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

set(SOURCE_FILES parallel.cpp, sequential.cpp, utilities.h)
add_executable(parallel parallel.cpp utilities.h)
add_executable(sequential sequential.cpp utilities.h)