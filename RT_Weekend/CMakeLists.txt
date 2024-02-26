#CMake virsion requred 
cmake_minimum_required(VERSION 3.1.0)

# project info
project(RT_OneWeekend VERSION 3.0.0 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 11)
add_executable(program RT_OneWeekend/chapter7.cpp)