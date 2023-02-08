cmake_minimum_required(VERSION 3.3)
project(parallel-bigrams-trigrams)

set(CMAKE_CXX_STANDARD 11)

# try also to compile and execute without: -fopenmp
# in Linux just use -fopenmp, without -lomp and -Xpreprocessor
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
set(CMAKE_CXX_LINKER_WRAPPER_FLAG "-fopenmp")

set(SOURCE_FILES main.cpp)
add_executable(main main.cpp)