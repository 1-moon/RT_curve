#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"
#include "utility.h"
#include "material.h"
#include <iostream>


class Camera {
public:
	// image data 
	double aspect_ratio = 0.0;
	int image_width = 0;
	int samples_per_pixel = 0;
	int max_depth = 0;
	// camera frame  
	point3 lookFrom = point3(0, 0, 0);
	point3 lookAt = point3(0, 0, 0);
	Vec3 up_vec = Vec3(0, 0, 0);
	double fov = 0; // Field-of-view in degrees



	void render(const Hittable_list& world) {
		init();
		std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

		for (int j = 0; j < image_height; j++) {
			std::cerr << "Scanlines remaining: " << (image_height - j)<< '\n';
			for (int i = 0; i < image_width; i++) {
				color pixel_color(0, 0, 0);
				// Apply anti-aliasing method   
				for (int sample_ray = 0; sample_ray < sample_rays_per_pixel; ++sample_ray) {

					Ray r = get_ray(i, j);
					//// Generate random value [0, 1] 
					//double random_u = static_cast<double>(rand()) / (RAND_MAX + 1.0);
					//double random_v = static_cast<double>(rand()) / (RAND_MAX + 1.0);

					//// scalars that vary for each pixel to iterate over the entire plane.
					//auto u = (i + random_u) / (image_width - 1);	// 0~1
					//auto v = (j + random_v) / (image_height - 1);	// 0~1
					//// Create a ray from the camera origin to the pixel on the virtual viewport
					//Ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
					pixel_color += ray_color(r, world, max_depth);
				}
				write_color(std::cout, pixel_color, sample_rays_per_pixel);
			}
		}
		std::cerr << "\nDone.\n";
	}



private:
	int image_height=0;
	// pixel data 
	point3 pixel_loc_00;	// location at pixel(0,0)
	Vec3 pixel_u_vec;			// offset to the right
	Vec3 pixel_v_vec;			// offset to below

	Vec3 frame_u, frame_v, frame_w; // camera frmae unit vectors

	// virtual viewport
	point3 camera_origin;
	double focus_distance = 10;

	// ray data
	int sample_rays_per_pixel = 10;
	// camera coordinates 
	  



	void init() {
		image_height = static_cast<int>(image_width / aspect_ratio);	// Estimate around 225px
		
		// Positionable camera: Determine viewport dimenstions  
		auto theta = fov * (pi / 180);			// Convert to radians
		auto half_h = tan(theta / 2);			// half height of the viewport
		auto unit_height = 2.0 * half_h;		// unit height of the viewport

		// Set real virtual viewport width and height
		auto virtual_view_height = unit_height * focus_distance;
		auto virtual_view_width = aspect_ratio * virtual_view_height;

		// Positionable camera: Calculate the basis vectors for the camera coordinate frame.  
		frame_w = Normalize(lookFrom - lookAt);
		frame_u = Normalize(cross(up_vec, frame_w));
		frame_v = cross(frame_u, frame_w);
		
		// Set camera data 
		camera_origin = lookFrom;
		
		// Calculate vector u & v that move horizontally and vertically along the viewport
		Vec3 viewport_u_vec = virtual_view_width * frame_u;
		Vec3 viewport_v_vec = virtual_view_height * frame_v;

		// Calculate pixel vectors 
		pixel_u_vec = viewport_u_vec / (image_width - 1); 
		pixel_v_vec = viewport_v_vec / (image_height - 1);

		// calculate the location of the upper left pixel
		auto upper_left_corner = camera_origin - (focus_distance * frame_w) - viewport_u_vec / 2 - viewport_v_vec / 2;
		pixel_loc_00 = upper_left_corner + 0.5 * (pixel_u_vec + pixel_v_vec);
		
	}

	Ray get_ray(int i, int j) {
		//  Get a anti-aliasied ray for pixel at (i,j)
		Vec3 pixel_sample = pixel_loc_00 + (i * pixel_u_vec) + (j * pixel_v_vec);
		point3 ray_origin = camera_origin;
		
		auto ray_dir = pixel_sample - ray_origin;
		return Ray(ray_origin, ray_dir);
	}


	color ray_color(const Ray& r, const Hittable_list world, int depth) {
		
		Hit_record record;

		//if (world.TestIntersection(r, Interval(0.0001, infinity), record)) {
		//	Ray scattered;
		//	color light_attenuation;
		//	color emittion_color = record.mat_ptr->emitted(record.u, record.v, record.int_p);
		//	
		//	if (!record.mat_ptr->scatter(r, record, light_attenuation, scattered)) 
		//		return emittion_color;

		//	color color_from_scatter = light_attenuation * ray_color(scattered, world, depth - 1);
		//	return emittion_color + color_from_scatter;

		//}else {
		//	// no object hit? -> return dark
		//	return color(0, 0, 0);
		//}



		if (world.TestIntersection(r, Interval(0.000001, infinity), record)) {
			/*
			//Diffuse reflection
			Vec3 random_reflect_ray = rec.int_p + rec.normal + random_unit_vector();
			return 0.5 * ray_color(Ray(rec.int_p, random_reflect_ray - rec.int_p), world); // // Set the attenuation factor to 0.5 for a 50% reduction per bounce
			*/
			// Reflection - material scatter
			Ray scattered;
			color light_attenuation;
			color emitted_color = record.mat_ptr->emitted(record.u, record.v, record.int_p);
			if (record.mat_ptr->scatter(r, record, light_attenuation, scattered))
				return light_attenuation * ray_color(scattered, world, depth - 1);
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

