/*
* color.h - How we write our pixel to the screen 
* with the utility function called right colour 
* that uses the Vec3 class 
*/
#ifndef COLOR_H
#define COLOR_H

#include "Vec3.h"
#include <iostream>

void write_color(std::ostream& out, color pixel_color) {
	// Write the translated [0,255] value of each color component
	out << static_cast<int>(255.999 * pixel_color.x()) << ' '
		<< static_cast<int>(255.999 * pixel_color.y()) << ' '
		<< static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

#endif