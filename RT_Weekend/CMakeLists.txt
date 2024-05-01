#CMake virsion requred 
cmake_minimum_required(VERSION 3.1.0)

# project info
project(RT_OneWeekend VERSION 3.0.0 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 11)

set ( curve
 RT_OneWeekend/bezierCurve.h
 RT_OneWeekend/camera.h
 RT_OneWeekend/color.h
 RT_OneWeekend/hittable.h
 RT_OneWeekend/hittable_list.h
 RT_OneWeekend/interval.h
 RT_OneWeekend/interval.cpp
 RT_OneWeekend/main.cpp
 RT_OneWeekend/ray.h 
 RT_OneWeekend/utility.h
 RT_OneWeekend/sphere.h
 RT_OneWeekend/vec3.h
 RT_OneWeekend/material.h
 RT_OneWeekend/texture.h
 RT_OneWeekend/bezierSurface.h
 RT_OneWeekend/triangle.h
 RT_OneWeekend/quadratic.h
)

include_directories(RT_OneWeekend)

add_executable(curve ${EXTERNAL} ${CURVE})

