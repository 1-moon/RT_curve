#CMake virsion requred 
cmake_minimum_required(VERSION 3.1.0)

# project info
project(RT_bezier VERSION 3.0.0 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 11)

set ( curve
 RT_bezier/bezierCurve.h
 RT_bezier/camera.h
 RT_bezier/color.h
 RT_bezier/hittable.h
 RT_bezier/hittable_list.h
 RT_bezier/interval.h
 RT_bezier/interval.cpp
 RT_bezier/main.cpp
 RT_bezier/ray.h 
 RT_bezier/utility.h
 RT_bezier/sphere.h
 RT_bezier/vec3.h
 RT_bezier/material.h
 RT_bezier/texture.h
 RT_bezier/bezierSurface.h
 RT_bezier/triangle.h
 RT_bezier/quadrangle.h
)

include_directories(RT_bezier)

add_executable(curve ${curve})

