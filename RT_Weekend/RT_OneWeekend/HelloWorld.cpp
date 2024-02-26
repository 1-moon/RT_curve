/*
	This file is a simple PPM image generator
	a.k.a. Hello World of Ray Tracing
*/

#include <iostream>
#include "Vec3.h"


int main()
{
	int width = 256;
	int height = 256;
	// add PPM header 
	std::cout << "P3\n" << width << " " << height << "\n255\n";
	
	for (int j = height - 1; j >= 0; j--) {
		for (int i = 0; i < width; i++) {
			// normalize the color
			float red = (float)i / float(width-1);
			float green = (float)j / float(height-1);
			float blue = 0.25;
			// convert float to bytes 
			int ir = static_cast<int>(255.99 * red);
			int ig = static_cast<int>(255.99 * green);
			int ib = static_cast<int>(255.99 * blue);
			std::cout << ir << " " <<
				ig << " " << ib << "\n";
		}
	}

	
}
