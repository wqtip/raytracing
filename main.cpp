#include "utils.h"

#include "sphere.h"
#include "object.h"
#include "camera.h"
#include "object_list.h"

using namespace std;

int main()
{
	
	std::ofstream out;
	object_list world;


	auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(make_shared<sphere>(point3(0, 0, -1000), 1000, ground_material));


	auto material1 = make_shared<metal>(color(random_double(),random_double(),random_double()),0.05);
	world.add(make_shared<sphere>(point3(0, 4, 2), 2, material1));
	auto material2 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(4, 0, 2), 2, material2));


	for (int i = -12; i <= 12; i++)
	{
		for (int j = -12; j <= 12; j++)
		{
			if (std::abs(i) + std::abs(j) >= 7)
			{
				point3 center(i, j, 0.2);
				shared_ptr<material> sphere_material;
				auto choose_mat = random_double();
				vec3 rand1 = vec3(random_double(), random_double(), random_double());
				vec3 rand2 = vec3(random_double(), random_double(), random_double());
				if (choose_mat < 0.7) {
									// diffuse
					auto albedo = hadamard_product(rand1, rand2);
					sphere_material = make_shared<lambertian>(albedo);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.9) {
									// metal
					auto albedo = rand1 * 0.5 + vec3(0.5, 0.5, 0.5);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else {
									// glass
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material_center = make_shared<lambertian>(color(random_double(), random_double(), random_double()));
	world.add(make_shared<sphere>(point3(0, 0, 1), 1, material_center));

	auto material3 = make_shared<metal>(color(random_double(), random_double(), random_double()), 0.05);
	world.add(make_shared<sphere>(point3(0, -4, 2), 2, material3));
	world.add(make_shared<sphere>(point3(-4, 0, 2), 2, material2));
	

	camera cam = camera();


	cam.aspect_ratio = 16.0 / 9.0;
	cam.img_width = 1200;
	cam.samples_per_pixel = 300;
	cam.max_depth = 50;
	cam.vertical_fov = 70;
	cam.camera_center = point3(0, -15, 10);
	cam.look_at = point3(0, 0, 0);
	cam.dof_lens_angle = 0.45;
	cam.focus_distance = std::sqrt(121 + 64) - 2;


	cam.init();
	cam.render(out, world);
}