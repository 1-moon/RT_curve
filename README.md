# Ray tracer with Curve primitive
This is a simple ray tracer adeveloped as a final project for COMP3931. 
It refers to many part of ray tracing tutorial by Peter shierly. 

## Basic Ray Tracing
The basic ray tracing model is as follows. A camera is placed in the world and rays are cast from the camera's position to points on an imaginary image plane. It also deals with Positionable camera, lighting, reflections and so on. In this case each ray represents a pixel in the output image and the resulting color of the ray determines the color output for the pixel. 
 
## Instruction - window  
Check:
  install visual studio for c++ 
  install cmake 
Initial:
 cmake -B build  (whenever you change your project CMakeLists.txt file)

1. go build folder and open .sln file using visual studio
  
3. open terminal and type cd .. (go into the folder CMakeLists file resides in)

4. cmake --build build; build/Debug/curve > image.ppm; start image.ppm
