#include "rtweekend.h"

#include "hittable_list.h"
#include "hittable.h"
#include "sphere.h"
#include "camera.h"

#include<iostream>

int main() {
	//World 

	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5)); // smaller sphere
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100)); // larger sphere

	// camera
	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.viewport_width = 400;

	cam.render(world);
}