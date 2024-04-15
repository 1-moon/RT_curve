

#include <iostream>
#include "utility.h" // Vec(point, color), Ray, interval(infinity) 
#include "color.h"	// write_color
#include "hittable_list.h"  
#include "camera.h"
#include "material.h"	// Lambertian
// Import primitive files   
#include "bezierCurve.h"
#include "bezierSurface.h"
#include "sphere.h"
#include "triangle.h"
#include "quadratic.h"
#include "bvh.h"

int main()
{
	//shared_ptr<Texture> checker = make_shared<Checker_texture>(color(0.2, 0.3, 0.1),
	//	color(0.9, 0.9, 0.9));
	
	std::vector<point3> controlPoints = {
		point3(278 - 75, 278 - 50, 100), point3(278 - 50, 278, 125), point3(278 - 25, 278 - 50, 150), point3(278, 278, 175),
		point3(278 + 25, 278 - 50, 100), point3(278 + 50, 278, 125), point3(278 + 75, 278 - 50, 150), point3(278 + 100, 278, 175),
		point3(278 - 75, 278 + 50, 200), point3(278 - 50, 278, 225), point3(278 - 25, 278 + 50, 250), point3(278, 278, 275),
		point3(278 + 25, 278 + 50, 200), point3(278 + 50, 278, 225), point3(278 + 75, 278 + 50, 250), point3(278 + 100, 278, 275)
	};

	// Material data 
	auto red = make_shared<Lambertian>(color(0.65, 0.05, 0.05));
	auto white = make_shared<Lambertian>(color(0.73, 0.73, 0.73));
	auto green = make_shared<Lambertian>(color(0.12, 0.45, 0.15));
	auto blue = make_shared<Lambertian>(color(0.1, 0.2, 0.5));

	auto light = make_shared<Luminant>(color(15, 15, 15));


	auto Material_lambertian = make_shared<Lambertian>( color(0.8, 0.1, 0.3) ); 
	auto Material_metal = make_shared<Metal>(color(0.8, 0.6, 0.2), 0.0);
	auto ground_material = make_shared<Lambertian>(color(0.1, 0.5, 1.0));
	
	auto surfaceMaterial = make_shared<Lambertian>(color(0.4, 0.2, 0.1));

	

	// Add primitives part 
	Hittable_list world;

	world.add(make_shared<Quadratic>(point3(555, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), red));   
	world.add(make_shared<Quadratic>(point3(0, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), blue));  
	world.add(make_shared<Quadratic>(point3(343, 545, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), light));
	world.add(make_shared<Quadratic>(point3(0, 0, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
	world.add(make_shared<Quadratic>(point3(555, 555, 555), Vec3(-555, 0, 0), Vec3(0, 0, -555), white));
	world.add(make_shared<Quadratic>(point3(0, 0, 555), Vec3(555, 0, 0), Vec3(0, 555, 0), white));


	world.add(make_shared<BezierSurface>(controlPoints, red));

	//shared_ptr<Hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), white);
	//box1 = make_shared<rotate_y>(box1, 15);
	//box1 = make_shared<translate>(box1, Vec3(265, 0, 295));
	//world.add(box1);

	//world.add(make_shared<Triangle>(point3(-1.5, 0, -1.5), point3(-1.5, 1, -0.5), point3(-1.5, -1, 0.5),surfaceMaterial));
	//world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100, make_shared<Lambertian>(checker)));
	//world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));
	//world.add(make_shared<Sphere>(point3(0, 9, 0), 3, lightening));
	//world.add(make_shared<Sphere>(point3(265, 300, 295), 50, red));
	//world.add(make_shared<Sphere>(point3(3, 2, 0), 2, Material_lambertian));
	 
	//world.add(make_shared<Sphere>(point3(-1, 0, -1), 0.5, Material_metal));

	Camera camera;
	// Set image data
	camera.aspect_ratio = 16.0 / 9.0;
	camera.image_width = 400;

	camera.lookFrom = point3(278, 278, -800);
	camera.lookAt = point3(278, 278, 0);
	camera.up_vec = Vec3(0, 1, 0);

	camera.fov = 40;
	camera.samples_per_pixel = 200;
	camera.max_depth = 50;

	// Set camera data
	camera.render(world);


}