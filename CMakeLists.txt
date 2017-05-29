cmake_minimum_required(VERSION 3.7)
project(graphs)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(graphs ${SOURCE_FILES})

target_link_libraries(graphs X11)
