
#include "rtweekend.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

// #include "color.h"
// #include "vec3.h"
// #include "ray.h"

//#include <iostream>

// double sphere_hit(const point3& center, double radius, const ray& r) {
//     vec3 oc = center - r.origin();
//     auto a = r.direction().length_squared();
//     auto h = dot(r.direction(), oc);
//     auto c = oc.length_squared() - radius*radius;
//     auto delta = h*h - a*c;

//     if (delta < 0) {
//         return -1.0;
//     } else {
//         return (h - std::sqrt(delta)) / a;
//     }
// }

color ray_color(const ray& r, const hittable& world) {
    hit_info info;
    if (world.hit(r, 0, infinity, info)) {
        return .5 * (info.normal + color(1, 1, 1));
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = .5*(unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a*color(0.5, .7, 1.0);
}

int main() {

    // Setting up the image dimensions according to our aspect ratio

    auto aspect_ratio = 16.0 / 9.0;
    int imageW = 400;

    int imageH = int(imageW / aspect_ratio);
    imageH = (imageH < 1) ? 1 : imageH;

    // World

    hittable_list world;

    world.add(make_shared<sphere>(point3(0, 0, -1), .5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Setting up the camera

    auto focal_length = 1.0;
    auto viewportH = 2.0;
    auto viewportW = viewportH * (double(imageW) / imageH);
    auto camera_center = point3(0, 0, 0);

    auto viewport_u = vec3(viewportW, 0, 0);
    auto viewport_v = vec3(0, -viewportH, 0 );

    auto pixel_delta_u = viewport_u / imageW;
    auto pixel_delta_v = viewport_v / imageH;

    // Placing upper-left pixel

    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel100_loc = viewport_upper_left + .5 * (pixel_delta_u + pixel_delta_v);

    std::cout << "P3\n" << imageW << " " << imageH << "\n255\n";

    for (int j = 0; j < imageH; j++) {
        std::clog << "\rScanlines remaining: " << (imageH - j) << ' ' << std::flush;
        for (int i = 0; i < imageW; i++) {
            auto pixel_center = pixel100_loc + (i * pixel_delta_u) + (j* pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";

}
