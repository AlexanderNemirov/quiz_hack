cmake_minimum_required(VERSION 3.21.3)
project(quiz_hack)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-O3")

add_executable(quiz_hack main.cpp )
target_link_libraries(quiz_hack PRIVATE pthread)
