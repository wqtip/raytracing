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

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), -(b + 0.9 * random_double()), 0.2);

			if (length(center - point3(4, 0.2, 0)) > 0.9) {
				shared_ptr<material> sphere_material;
				color rand1(random_double(), random_double(), random_double());
				color rand2(random_double(), random_double(), random_double());
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

	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0, 0, 1), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(point3(-4, 0, 1), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4, 0, 1), 1.0, material3));


	camera cam = camera();


	cam.aspect_ratio = 16.0 / 9.0;
	cam.img_width = 1200;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.vertical_fov = 20;
	cam.camera_center = point3(13, -3, 2);
	cam.look_at = point3(0, 0, 0);
	cam.dof_lens_angle = 0.6;
	cam.focus_distance = 10.0;


	cam.init();
	cam.render(out, world);
}