#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"
#include "rtweekend.h"
#include <iostream>

class camera {
	public: 
		double aspect_ratio = 1.0;	// Ratio of image width over height 
		int image_width = 100;		// Rendered image width in pixel count 

		void render(const hittable& world) {
			initialize();
			  
			std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

			for (int j =0; j < image_height; ++j) {
				std::cerr << "Scanlines remaining: " << (image_height -j) << '\n';
				for (int i = 0; i < image_width; ++i) {
					auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
					auto ray_direction = pixel_center - center;
					ray r(center, ray_direction);

					color pixel_color = ray_color(r, world);
					write_color(std::cout, pixel_color);
				}
			}
			std::cerr << "\nDone.\n";
		}
	private: 
		int image_height;		// Rendered image height 
		point3 center; 			// Camera center
		point3 pixel00_loc;		// Location of the top-left pixel 0, 0
		Vec3 pixel_delta_u;		// offset to pixel to the right
		Vec3 pixel_delta_v ;	// offset to pixel below

		void initialize() {}
		
		color ray_color(const ray& r, const hittable& world) const {
			hit_record rec;
			
			if (world.hit(r, interval(0, infinity), rec)) {
				return 0.5 * (rec.normal + color(1, 1, 1));
			}

			Vec3 unit_direction = unit_vector(r.direction());
			auto t = 0.5 * (unit_direction.y() + 1.0);
			return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
		}

};

#endif

