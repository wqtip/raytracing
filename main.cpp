#include "utils.h"

#include "sphere.h"
#include "object.h"
#include "camera.h"
#include "object_list.h"


int main() {
    std::ofstream out;
    object_list world;

  
    auto mirror_floor = make_shared<metal>(color(0.3, 0.3, 0.5), 0.0);
    auto glass = make_shared<dielectric>(1.5);
    auto chrome = make_shared<metal>(color(0.9, 0.9, 0.95), 0.0);
    auto warm_gold = make_shared<metal>(color(1.0, 0.84, 0.0), 0.02);
    auto cool_silver = make_shared<metal>(color(0.8, 0.83, 0.9), 0.03);
    auto diffuse_cyan = make_shared<lambertian>(color(0.35, 0.8, 0.9));
    auto diffuse_magenta = make_shared<lambertian>(color(0.9, 0.35, 0.7));


    world.add(make_shared<sphere>(point3(0, 0, -10000), 10000, mirror_floor)); 

    point3 globe_center(0, 0, 15);
    double globe_R = 5.0;
    world.add(make_shared<sphere>(globe_center, globe_R, glass)); 
    world.add(make_shared<sphere>(globe_center, 2.0, chrome));


    int    n_orbit = 8;
    double orbit_R = 12;   
    double orbit_z = 15;    
    double tilt = 18.0 * M_PI / 180;

    for (int m = 0; m < n_orbit; ++m) {
        double t = (2.0 * M_PI * m) / n_orbit;
        double y0 = orbit_R * std::cos(t);
        double z0 = orbit_R * std::sin(t);
        double y = y0 * std::cos(tilt) - z0 * std::sin(tilt);
        double z = y0 * std::sin(tilt) + z0 * std::cos(tilt);
        point3 c(0.0, y, orbit_z + z);

        shared_ptr<material> mat;
        if (m % 2 == 0) {
            mat = (m % 4 == 0) ? warm_gold : cool_silver;
        }
        else {
            mat = (m % 4 == 1) ? diffuse_cyan : diffuse_magenta;
        }

        world.add(make_shared<sphere>(c, 1.0, mat)); 
    }

 
    auto make_pillar = [&](double x) {
        world.add(make_shared<sphere>(point3(x, -3.0, 1.0), 1.0, glass)); 
        world.add(make_shared<sphere>(point3(x, -3.0, 3.2), 1.0, glass)); 
        world.add(make_shared<sphere>(point3(x, -3.0, 5.4), 1.0, glass)); 
    };
    make_pillar(-12.0); 
    make_pillar(12.0); 

 
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.img_width = 1200;
    cam.samples_per_pixel = 1000;   
    cam.max_depth = 60;
    cam.vertical_fov = 34;

    cam.camera_center = 2 * point3(30, -20, 14); 
    cam.look_at = globe_center;        
    cam.dof_lens_angle = 0.15;             
    cam.focus_distance = length(cam.look_at - cam.camera_center);

    cam.init();
    cam.render(out, world);
}