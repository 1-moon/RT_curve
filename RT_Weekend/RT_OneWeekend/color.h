/*
* color.h - How we write our pixel to the screen 
* with the utility function called right colour 
* that uses the Vec3 class 
*/
#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

void write_color(std::ostream& out, color pixel_color, int samples_rays_per_pixel) {
	// Extract each color of pixel 
	auto r_pixel = pixel_color.x();
	auto g_pixel = pixel_color.y();
	auto b_pixel = pixel_color.z();

	// Divide the color by the number of rays per pixel 
	auto scale = 1.0 / samples_rays_per_pixel;
	r_pixel = r_pixel * scale;
	g_pixel = g_pixel * scale; 
	b_pixel = b_pixel * scale; 

	// Write the translated [0,255] value of each color component
	out << static_cast<int>(255.999 * r_pixel) << ' '
		<< static_cast<int>(255.999 * g_pixel) << ' '
		<< static_cast<int>(255.999 * b_pixel) << '\n';
}	

#endif