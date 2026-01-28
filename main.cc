
#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

int main() {

    // Setting up world and adding both ground and a sphere to it

    hittable_list world;

    auto material_red = make_shared<lambertian>(color(.98, .1, .1));
    auto material_green = make_shared<lambertian>(color(.2, .98, .2));
    auto material_blue = make_shared<lambertian>(color(.6, .65, .98));

    // auto material_left = make_shared<dielectric>(1.50);
    auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    auto shiny_metal = make_shared<metal>(color(.8, .8, 1), 0.01);

    world.add(make_shared<sphere>(point3(0, -100.5, -1.0), 100.0, shiny_metal));


    world.add(make_shared<sphere>(point3(0, 0.3, -1.4), .8, material_blue));

    world.add(make_shared<sphere>(point3(-1.5, -0.15, .3), .4, material_green));
    world.add(make_shared<sphere>(point3(-2, -0.05, -3), .5, material_green));
    world.add(make_shared<sphere>(point3(.8, -0.2, -4), .3, material_green));
    world.add(make_shared<sphere>(point3(1.2, .05, 1.8), .6, shiny_metal));

    world.add(make_shared<sphere>(point3(-.8, 0.7, -4), .6, material_bubble));
    world.add(make_shared<sphere>(point3(1.2, 1.4, -3), .3, material_bubble));
    world.add(make_shared<sphere>(point3(-0.6, 1.3, .5), .4, material_bubble));
    world.add(make_shared<sphere>(point3(3, 1.1, 1.7), .4, material_bubble));
    
    // Setting up camera
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.imageW = 1000;
    cam.spp = 300;
    cam.max_depth = 50;

    cam.vfov = 45;
    cam.lookfrom = point3(-5, 1, 1);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = .3;
    cam.focus_dist    = 5.0;

    cam.render(world);

}
