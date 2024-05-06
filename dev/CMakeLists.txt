cmake_minimum_required(VERSION 3.10)

project(main)

include_directories(include)

add_executable(main src/main.cpp src/helloworld.cpp src/sum.cpp)
