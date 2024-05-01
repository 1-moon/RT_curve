#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"
#include "utility.h"
#include "material.h"
#include <iostream>


class Camera {
public:
	// changable image data 
	int image_width = 0;
	int samples_per_pixel = 0;
	int max_depth = 00;
	// changable camera frame  
	point3 lookFrom = point3(0, 0, 0);
	point3 lookAt = point3(0, 0, 0);
	Vec3 up_vec = Vec3(0, 0, 0);
	double fov = 0; // Field-of-view in degrees
	
	void render(const Hittable_list& world) {
		// Initialize the camera settng 
		init();
		// Print the PPM image header
		std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
		// Render the image  
		for (int j = 0; j < image_height; ++j) {
			std::cerr << "Scanlines remaining: " << (image_height - j)<< '\n';
			for (int i = 0; i < image_width; ++i) {
				color pixel_color(0, 0, 0);	 
				// Anti-aliasing: multiple rays within a pixel
				for (int sample_ray = 0; sample_ray < samples_per_pixel; ++sample_ray) {
					Ray r = anti_aliasied_ray(i, j);
					// Accumulate the color of the pixel
					pixel_color += ray_color(r, world, max_depth);
				}

				write_color(std::cout, pixel_color, samples_per_pixel);
			}
		}
		std::cerr << "\nDone.\n";
	}

private:
	int image_height=0;
	double aspect_ratio = 16.0 / 9.0;	// Set default aspect ratio
	// pixel data 
	point3 pixel_loc_00;		// location at pixel(0,0)
	Vec3 pixel_delta_u;			// offset to the right
	Vec3 pixel_delta_v;			// offset to below

	Vec3 frame_u, frame_v, frame_w; // camera frmae unit vectors

	// virtual viewport
	point3 primary_ray_start;
	double d = 10;		// distance from the camera to the viewport

	Ray anti_aliasied_ray(int i, int j) {

		// Generate random value [0, 1] 
		double random_u = static_cast<double>(rand()) / (RAND_MAX + 1.0);
		double random_v = static_cast<double>(rand()) / (RAND_MAX + 1.0);
		// Generate random value using pixel vectors u,v in a pixel
		point3 random_value = random_u * pixel_delta_u + random_v * pixel_delta_v;

		Vec3 pixel_center = pixel_loc_00 + (i * pixel_delta_u) + (j * pixel_delta_v);

		// Get a random point in the pixel
		auto pixel_sample = pixel_center + random_value;

		auto ray_dir = pixel_sample - primary_ray_start;
		//  Get a anti-aliasied ray for pixel at (i,j)
		return Ray(primary_ray_start, ray_dir);
	}


	void init() {
		image_height = static_cast<int>(image_width / aspect_ratio);	// Estimate around 225px
		
		// Positionable camera: Determine viewport dimenstions  
		double theta = fov * (pi / 180);			// Convert to radians
		double half_h = tan(theta / 2);			// half height of the viewport
		auto unit_height = 2.0 * half_h;		// unit height of the viewport

		// Set real virtual viewport width and height
		auto viewport_height = unit_height * d;
		auto viewport_width = aspect_ratio * viewport_height;

		// Positionable camera: Calculate the basis vectors for the camera coordinate frame.  
		frame_w = Normalize(lookFrom - lookAt);
		frame_u = Normalize(cross(up_vec, frame_w));
		frame_v = cross(frame_u, frame_w);
		
		// Set camera data 
		primary_ray_start = lookFrom;
		
		// Calculate vector u & v that move horizontally and vertically along the viewport
		Vec3 viewport_u_vec = viewport_width * frame_u;
		Vec3 viewport_v_vec = viewport_height * frame_v;

		// Calculate pixel vectors 
		pixel_delta_u = viewport_u_vec / (image_width - 1);
		pixel_delta_v = viewport_v_vec / (image_height - 1);

		// calculate the location of the upper left pixel
		auto upper_left_corner = primary_ray_start - (d * frame_w) - viewport_u_vec / 2 - viewport_v_vec / 2;
		pixel_loc_00 = upper_left_corner + 0.5 * (pixel_delta_u + pixel_delta_v);
		
	}


	color ray_color(const Ray& r, const Hittable_list& world, int depth) {
		
		if (depth <= 0)
			return color(0, 0, 0);

		Hit_record record;
		// hit an object?
		if (world.TestIntersection(r, Interval(0.0001, infinity), record)) {
			const double bias = 0.001;	// bias variable for shaodw acne  
			Ray scattered;
			color light_attenuation;
			color color_from_scatter;
;			color emittion_color = record.mat_ptr->emitted(record.u, record.v, record.int_p);
			// if the material emits light ? no need to scatter 
			if (!record.mat_ptr->scatter(r, record, light_attenuation, scattered)) 
				return emittion_color;
			else	// apply bias to avoid shadow acne
				color_from_scatter = light_attenuation * ray_color(Ray(record.int_p + bias * record.normal, scattered.direction()), world, depth - 1);
			return emittion_color + color_from_scatter;

		}else {
			// no object hit? -> return dark
			return color(0, 0, 0);
		}
		
		// ====================== testing with no light ========================== 
		//if (world.TestIntersection(r, Interval(0.00001, infinity), record)) {
		//	/*
		//	//Diffuse reflection
		//	Vec3 random_reflect_ray = rec.int_p + rec.normal + random_unit_vector();
		//	return 0.5 * ray_color(Ray(rec.int_p, random_reflect_ray - rec.int_p), world); // // Set the attenuation factor to 0.5 for a 50% reduction per bounce
		//	*/
		//	// Reflection - material scatter
		//	Ray scattered;
		//	color light_attenuation;
		//	color emitted_color = record.mat_ptr->emitted(record.u, record.v, record.int_p);
		//	if (record.mat_ptr->scatter(r, record, light_attenuation, scattered))
		//		return light_attenuation * ray_color(scattered, world, depth - 1);
		//	return color(0, 0, 0);

		//}
		//else {
		//	// Return background color 
		//	Vec3 unit_direction = Normalize(r.direction());
		//	auto t = 0.5 * (unit_direction.y() + 1.0);	 // add 1 to ensure it's above zero, multiply by 0.5 to get a num between 0~1
		//	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
		//}
		// ====================== testing with no light ==========================
	
	}
};
#endif





