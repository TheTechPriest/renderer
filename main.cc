
#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

int main() {

    // Setting up world and adding both ground and a sphere to it

    hittable_list world;

    world.add(make_shared<sphere>(point3(0, 0, -1), .5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));
    
    // Setting up camera
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.imageW = 400;
    cam.spp = 100;
    cam.max_depth = 50;

    cam.render(world);

}
