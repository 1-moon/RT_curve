

#include <iostream>
#include "ray.h"
#include "vec3.h"
#include "color.h"
#include "hittable_list.h"
#include "bezierCurve.h"



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
	world.add(make_shared<BezierCurve>(point3(-1, 0, -1), point3(-0.5, 0.5, -1),
							point3(0.5, -0.5, -1), point3(1, 0, -1) ));

	// Set defalut virtual viewport
	auto origin = point3(0, 0, 0);	// camera origin
	auto focal_length = 1.0;
	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio * viewport_height;
	auto lower_left_corner = origin - Vec3(viewport_width / 2, viewport_height / 2, focal_length); 

	auto horizontal = Vec3(viewport_width, 0.0, 0.0);
	auto vertical = Vec3(0.0, viewport_height, 0.0);

	// Render part  
	int sample_rays_per_pixel = 10;
	std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; j--) {
		std::cerr << "Scanlines remaining: " << j << '\n';
		for (int i = 0; i < image_width; i++) {
			color pixel_color(0, 0, 0);
			// Apply anti-aliasing method   
			for (int sample_ray = 0; sample_ray < sample_rays_per_pixel; ++sample_ray) {
				// Generate random value [0, 1) 
				double random_u = static_cast<double>(rand()) / (RAND_MAX + 1.0);
				double random_v = static_cast<double>(rand()) / (RAND_MAX + 1.0);

				// scalars that vary for each pixel to iterate over the entire plane.
				auto u = (i + random_u) / (image_width - 1);	// 0~1
				auto v = (j + random_v) / (image_height - 1);	// 0~1
				// Create a ray from the camera origin to the pixel on the virtual viewport
				Ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
				pixel_color += ray_color(r, world);
			}
			write_color(std::cout, pixel_color, sample_rays_per_pixel);
		}
	}
	std::cerr << "\nDone.\n";

}