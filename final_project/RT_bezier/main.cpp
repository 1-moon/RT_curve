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
#include "quadrangle.h"


int main()
{
	// Set up curve data 
	point3 p0 = point3(278 - 75, 278 - 50, 100);
	point3 p1 = point3(278 + 100, 278, 175);
	point3 p2 = point3(278 + 75, 278 + 50, 250);
	point3 p3 = point3(278 - 50, 278, 225);
	// Set up surface data
	std::vector<point3> controlPoints2 = {
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

	// Set up the space   
	Hittable_list world;
	world.add(make_shared<Quadrangle>(point3(343, 450, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), light));
	world.add(make_shared<Quadrangle>(point3(0, 200, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
	
	// Call curve  
	//world.add(make_shared<BezierCurve>(p0, p1, p2, p3, green));
	// Call surface
	world.add(make_shared<BezierSurface>(controlPoints2, red));


	Camera camera;
	// Set image data
	camera.image_width = 400;

	camera.lookFrom = point3(278, 450, -500);
	camera.lookAt = point3(278, 278, 0);
	camera.up_vec = Vec3(0, 1, 0);

	camera.fov = 40;
	camera.samples_per_pixel = 200;
	camera.max_depth = 50;	// Maximum number of bounces into scene. 
	 
	// Set camera data
	camera.render(world);


}