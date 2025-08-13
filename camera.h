#ifndef CAMERA_H
#define CAMERA_H

#include "object.h"
#include "object_list.h"
#include "material.h"

class camera {
private:
	int img_height;
	double viewport_width;
	vec3 viewport_delta_w;
	vec3 viewport_delta_h;
	point3 viewport_upper_left;
	point3 upper_left_pixel;
	vec3 cam_basis_view;
	vec3 cam_basis_2;
	vec3 cam_basis_3;
	double double_max = std::numeric_limits<double>::max();
	double lens_radius;
	static void write_color(std::ostream& out, color c)
	{
		int r = int(std::sqrt(c.x()) * 255.999);
		int g = int(std::sqrt(c.y()) * 255.999);
		int b = int(std::sqrt(c.z()) * 255.999);
		out << r << " " << g << " " << b << "\n";
	}
	color ray_color(ray& r, int depth, object_list& world) const {
		if (depth < 0) return color(0, 0, 0);
		hit_record rec;
		if (world.hit(r, 0.0001, double_max, rec))
		{
			color attenuation;
			ray scattered;
			if (rec.mat->scatter(r, rec, attenuation, scattered))
			{
				return hadamard_product(attenuation, ray_color(scattered, depth - 1, world));
			}
			return color(0, 0, 0);
		}
		double a = 0.5 * (unit_vector(r.direction()).z() + 1.0);
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}
public:
	int samples_per_pixel = 100;
	int max_depth = 50;
	double aspect_ratio = 16.0 / 9.0;
	int img_width = 600;
	double viewport_height = 2.0;
	point3 camera_center = point3(0, 0, 0);
	point3 look_at = point3(0, 1, 0);
	vec3 cam_up;
	double vertical_fov = 90;
	double dof_lens_angle = 0.5;
	double focus_distance = 5;
	camera()
	{
	}
	void init()
	{
		double fov_radians = vertical_fov * M_PI / 180;
		viewport_height = focus_distance * 2 * std::tan(fov_radians / 2);
		img_height = int(img_width / aspect_ratio);
		img_height = std::max(img_height, 1);
		viewport_width = viewport_height * aspect_ratio;

		cam_basis_view = unit_vector(look_at - camera_center);
		cam_up = std::fabs(cam_basis_view.z()) > 0.9999 ? vec3(0, 1, 0) : vec3(0, 0, 1);
		cam_basis_2 = unit_vector(cross(cam_basis_view, cam_up));
		cam_basis_3 = cross(cam_basis_2, cam_basis_view);

		vec3 viewport_w_vec = viewport_width * cam_basis_2;
		viewport_delta_w = viewport_w_vec / double(img_width);
		vec3 viewport_h_vec = -viewport_height * cam_basis_3;
		viewport_delta_h = viewport_h_vec / double(img_height);

		lens_radius = focus_distance * std::tan(dof_lens_angle * M_PI / 360);
		viewport_upper_left = camera_center + focus_distance * cam_basis_view - viewport_w_vec / 2 - viewport_h_vec / 2;
		upper_left_pixel = viewport_upper_left + viewport_delta_w / 2 + viewport_delta_h / 2;
	}
	void render(std::ofstream& out, object_list& world)
	{
		out.open("output.ppm");
		out << "P3\n" << img_width << " " << img_height << "\n255\n";

		/*ray curr(camera_center, vec3(0, 1, 0));
		write_color(out, ray_color(curr, world));*/
		for (int j = 0; j < img_height; j++)
		{
			for (int i = 0; i < img_width; i++)
			{
				if (i == 0 || i == img_width / 3 || i == 2 * img_width / 3) std::cerr << "percent complete: " << (double(j) * img_width + i) / (img_width * img_height) * 100 << "%\n" << std::flush;
				color curr_color(0, 0, 0);
				for (int sample = 0; sample < samples_per_pixel; sample++)
				{
					vec3 offset = random_double(-0.5, 0.5) * viewport_delta_w + random_double(-0.5, 0.5) * viewport_delta_h;
					std::vector<double> disk_coordinates = random_in_disk();
					vec3 random_point_on_lens = camera_center + lens_radius * disk_coordinates[0] * cam_basis_2 + lens_radius * disk_coordinates[1] * cam_basis_3;
					ray curr_ray(random_point_on_lens, unit_vector(upper_left_pixel + j * viewport_delta_h + i * viewport_delta_w + offset - random_point_on_lens));
					curr_color = curr_color + ray_color(curr_ray, max_depth, world);
				}
				write_color(out, curr_color / samples_per_pixel);
			}
		}
		out.close();
	}
	
};


#endif