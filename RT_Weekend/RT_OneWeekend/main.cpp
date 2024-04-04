

#include <iostream>
#include "utility.h" // Vec(point, color), Ray, interval(infinity) 
#include "color.h"	// write_color
#include "hittable_list.h"  
#include "camera.h"
#include "material.h"	// Lambertian
// Import primitive files   
#include "bezierCurve.h"
#include "sphere.h"


int main()
{
	//shared_ptr<Texture> checker = make_shared<Checker_texture>(color(0.2, 0.3, 0.1),
	//	color(0.9, 0.9, 0.9));

	// Material data 
	auto Material_lambertian = make_shared<Lambertian>( color(0.8, 0.1, 0.3) ); 
	auto Material_metal = make_shared<Metal>(color(0.8, 0.6, 0.2), 0.0);
	auto ground_material = make_shared<Lambertian>(color(0.1, 0.5, 1.0));
	auto lightening = make_shared<Luminant>(color(4, 4, 4));

	// Add primitives part 
	Hittable_list world;
	//world.add(make_shared<BezierCurve>(point3(0, 1, -1), point3(-0.5, 1, -3),
	//							point3(0.5, 1, -1), point3(1, 1, -1), Material_lambertian));


	//world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100, make_shared<Lambertian>(checker)));
	world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));
	world.add(make_shared<Sphere>(point3(0, 9, 0), 3, lightening));
	world.add(make_shared<Sphere>(point3(0, 2, 0), 2, Material_lambertian));
	//world.add(make_shared<Sphere>(point3(-1, 0, -1), 0.5, Material_metal));

	Camera camera;
	// Set image data
	camera.aspect_ratio = 16.0 / 9.0;
	camera.image_width = 400;

	camera.lookFrom = point3(26, 3, 6);
	camera.lookAt = point3(0, 2, 0);
	camera.up_vec = Vec3(0, 1, 0);

	camera.fov = 30;
	camera.samples_per_pixel = 100;
	camera.max_depth = 50;

	// Set camera data
	camera.render(world);


}