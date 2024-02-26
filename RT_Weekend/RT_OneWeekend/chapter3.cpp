#include <iostream>

#include "colour.h"


// returns the color of the background 
// ensure it's above zero then multiply by 0.5 to get a num betwen 0~1
color getColor(const Vec3& r) {
	Vec3 unit_direction = unit_vector(r);
	auto t = 0.5 * (unit_direction.y() + 1.0);	 // add 1 to ensure it's above zero, multiply by 0.5 to get a num between 0~1
	// color(111)-> white; color(0.5, 0.7, 1.0) -> a shade of blue
	// T getting close to 1 ? -> blue, T getting close to 0 ? -> white   
	return(1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
	//// Image data 
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;	// 400px
	const int image_height = static_cast<int>(image_width / aspect_ratio);	// 225px 

	// ========== Camera part ===============
	// virtual viewport
	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio * viewport_height;	//3.555
	auto focal_length = 1.0;

	auto origin = point3(0, 0, 0);
	auto lower_left_corner = origin - Vec3(viewport_width / 2, viewport_height / 2, focal_length);
	auto horizontal = Vec3(viewport_width, 0.0, 0.0);
	auto vertical = Vec3(0.0, viewport_height, 0.0);
	// ========== Render part ===============  
	std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; j--) {
		for (int i = 0; i < image_width; i++) {
			// normalize the color
			float u = (float)i / float(image_width - 1);
			float v = (float)j / float(image_height - 1);
			Vec3 vec(lower_left_corner + u * horizontal + v * vertical - origin);

			// this creates a 'color' variable called pixel_color
			color pixel_color = getColor(vec);
			write_color(std::cout, pixel_color);
		}
	}

	std::cerr << "\nDone.\n";
}