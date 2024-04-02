

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

	// Material data 
	auto Material_lambertian = make_shared<Lambertian>( color(0.8, 0.1, 0.3) ); 
	auto Material_metal = make_shared<Metal>(color(0.8, 0.6, 0.2), 0.0);
	auto ground_material = make_shared<Lambertian>(color(0.1, 0.5, 1.0));
	shared_ptr<Texture> checker = make_shared<Checker_texture>(color(0.2, 0.3, 0.1),
		color(0.9, 0.9, 0.9));
	// Add primitives part 
	Hittable_list world;
	world.add(make_shared<BezierCurve>(point3(0, 0, -1), point3(-0.5, 0.5, -3),
								point3(0.5, -0.5, -1), point3(1, 0, -1), Material_lambertian));


	//world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100, make_shared<Lambertian>(checker)));
	world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100, ground_material));
	//world.add(make_shared<Sphere>(point3(0, 0, -1), 0.5, Material_lambertian)); // smaller sphere
	//world.add(make_shared<Sphere>(point3(1, 0, -1), 0.5, Material_metal));
	//world.add(make_shared<Sphere>(point3(-1, 0, -1), 0.5, Material_metal));

	Camera camera;
	// Set image data
	camera.aspect_ratio = 16.0 / 9.0;
	camera.image_width = 400;

	// Set camera data
	camera.render(world);


}