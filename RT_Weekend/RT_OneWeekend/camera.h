#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"
#include "rtweekend.h"
#include <iostream>

class Camera {
	public: 
		double aspect_ratio = 1.0;	// Ratio of image width over height 
		int image_width = 100;		// Rendered image width in pixel count 
		
		Camera() {
			double viewport_height = 2.0;
			double viewport_width = aspect_ratio * viewport_height;	//3.555
			double focal_length = 1.0;

			point3 origin = point3(0, 0, 0);
			Vec3 lower_left_corner = origin - Vec3(viewport_width / 2, viewport_height / 2, focal_length);
			Vec3 horizontal = Vec3(viewport_width, 0.0, 0.0);
			Vec3 vertical = Vec3(0.0, viewport_height, 0.0);
		}

		
		void render(const Hittable& world) {
			initialize();
			  
			std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

			for (int j =0; j < image_height; ++j) {
				std::cerr << "Scanlines remaining: " << (image_height -j) << '\n';
				for (int i = 0; i < image_width; ++i) {
					auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
					auto ray_direction = pixel_center - camera_from;
					Ray r(camera_from, ray_direction);

					color pixel_color = ray_color(r, world);
					write_color(std::cout, pixel_color,10);
				}
			}
			std::cerr << "\nDone.\n";

		}

		Ray get_ray_from_pixel(double u, double v) {
			return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
		}
		point3 origin;
		Vec3 lower_left_corner;
		Vec3 horizontal;
		Vec3 vertical;
	private: 
		int image_height;		// Rendered image height 
		point3 camera_from;		// Camera location
		point3 pixel00_loc;		// Location of the top-left pixel (0,0)
		Vec3 pixel_delta_u;		// offset to pixel to the right
		Vec3 pixel_delta_v ;	// offset to pixel below



		void initialize() {
		}


		color ray_color(const Ray& r, const Hittable& world) const {
			Hit_record rec;
			
			if (world.TestIntersection(r, interval(0, infinity), rec)) {
				return 0.5 * (rec.normal + color(1, 1, 1));
			}

			Vec3 unit_direction = Normalize(r.direction());
			auto t = 0.5 * (unit_direction.y() + 1.0);
			return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
		}

};

#endif

