#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"
#include "utility.h"
#include "material.h"
#include <iostream>

class Camera {
public:
	// Image data 
	double aspect_ratio;
	int image_width;


	void render(const Hittable_list& world) {
		init();
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
					pixel_color += ray_color(r, world,10);
				}
				write_color(std::cout, pixel_color, sample_rays_per_pixel);
			}
		}
		std::cerr << "\nDone.\n";
	}


private:
	int image_height;
	//virtual viewport
	double viewport_height;
	double viewport_width;
	double focal_length;
	point3 origin ;
	Vec3 lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical ;
	// ray data
	int sample_rays_per_pixel = 10;

	void init() {
		image_height = static_cast<int>(image_width / aspect_ratio);	// Estimate around 225px

		// Set viewport size 
		viewport_height = 2.0;  
		viewport_width = aspect_ratio * viewport_height;

		// Set camera data 
		focal_length = 1.0; 
		origin = point3(0, 0, 0); 
		lower_left_corner = origin - Vec3(viewport_width / 2, viewport_height / 2, focal_length);

		// 뷰포트를 따라 가로와 세로로 이동하는 벡터 계산
		horizontal = Vec3(viewport_width, 0.0, 0.0);
		vertical = Vec3(0.0, viewport_height, 0.0);
	}

	color ray_color(const Ray& r, const Hittable_list world, int depth) {

		Hit_record record;
		// kick things off by setting the initial range 
		if (world.TestIntersection(r, Interval(0.0001, infinity), record)) {
			/*
			//Diffuse reflection
			Vec3 random_reflect_ray = rec.int_p + rec.normal + random_unit_vector();
			return 0.5 * ray_color(Ray(rec.int_p, random_reflect_ray - rec.int_p), world); // // Set the attenuation factor to 0.5 for a 50% reduction per bounce
			*/

			// Reflection - material scatter
			Ray scattered;
			color attenuation;
			if (record.mat_ptr->scatter(r, record, attenuation, scattered))
				return attenuation * ray_color(scattered, world, depth - 1);
			return color(0, 0, 0);

		}
		else {
			// Return background color 
			Vec3 unit_direction = Normalize(r.direction());
			auto t = 0.5 * (unit_direction.y() + 1.0);	 // add 1 to ensure it's above zero, multiply by 0.5 to get a num between 0~1
			return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
		}

	}
};
#endif

