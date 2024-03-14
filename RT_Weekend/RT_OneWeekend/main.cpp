

#include <iostream>
#include "ray.h"
#include "vec3.h"
#include "color.h"
#include "hittable_list.h"
#include "curve.h"



// Create a pixel with this formular 
color ray_color(const Ray& r, const Hittable_list world) {

	Hit_record rec;

	// kick things off by setting the initial range to 0~infinity
	if (world.TestIntersection(r, interval(0, infinity), rec)) {	
		// hit shaded based on the normal of the closest obj, 
		// passeed back from the output parameter
		return 0.5 * (rec.normal + color(1, 1, 1));
	}

	Vec3 unit_direction = Normalize(r.direction());
	
	// add 1 to ensure it's above zero, multiply by 0.5 to get a num between 0~1
	auto t = 0.5 * (unit_direction.y() + 1.0);	 
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}


int main() {
	// Image part 
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);

	// World part 
	Hittable_list world;
	world.add(make_shared<Curve>(point3(-1, 1/2, -1), point3(-1, 1 / 2, -1),
							point3(-1, 1 / 2, -1), point3(-1, 1 / 2, -1) ));

	// Virtual viewport
	auto origin = point3(0, 0, 0);	// camera origin
	auto focal_length = 1.0;
	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio * viewport_height;
	auto lower_left_corner = origin - Vec3(viewport_width / 2, viewport_height / 2, focal_length); 

	auto horizontal = Vec3(viewport_width, 0.0, 0.0);
	auto vertical = Vec3(0.0, viewport_height, 0.0);

	// Render part  
	std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; j--) {
		std::cerr << "Scanlines remaining: " << j << '\n';
		for (int i = 0; i < image_width; i++) {
			auto u = double(i) / (image_width - 1);		// 0~1
			auto v = double(j) / (image_height - 1);	// 0~1
			// Generate a ray 
			Ray CameraRay(origin, 
				lower_left_corner + u * horizontal + v * vertical - origin);
			color pixel_color = ray_color(CameraRay, world);

			write_color(std::cout, pixel_color);
		}
	}
	std::cerr << "\nDone.\n";


	// render part 
}