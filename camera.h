#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera {

    public:
        double aspect_ratio = 1.0;
        int imageW = 100;
        // Samples per pixel - Antialiasing setting
        int spp = 10;

        void render(const hittable& world) {
            initialize();

            std::cout << "P3\n" << imageW << " " << imageH << "\n255\n";

            for (int j = 0; j < imageH; j++) {
                std::clog << "\rScanlines remaining: " << (imageH - j) << ' ' << std::flush;
                for (int i = 0; i < imageW; i++) {
                    color pixel_color(0, 0, 0);

                    //Sampling pixel color from surrounding rays
                    for (int sample = 0; sample < spp; sample++) {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, world);
                    }
                    write_color(std::cout, pixel_samples_scale * pixel_color);
                }
            }

            std::clog << "\rDone.                 \n";

        }

    private:
        int imageH;
        double pixel_samples_scale;
        point3 center;
        point3 origin;
        vec3 pixel_delta_u;
        vec3 pixel_delta_v;

        void initialize() {
            imageH = int(imageW / aspect_ratio);
            imageH = (imageH < 1) ? 1 : imageH;

            pixel_samples_scale = 1.0 / spp;

            center = point3(0, 0, 0);

            auto focal_length = 1.0;
            auto viewportH = 2.0;
            auto viewportW = viewportH * (double(imageW) / imageH);

            auto viewport_u = vec3(viewportW, 0, 0);
            auto viewport_v = vec3(0, -viewportH, 0);

            pixel_delta_u = viewport_u / imageW;
            pixel_delta_v = viewport_v / imageH;

            auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
            origin = viewport_upper_left + .5 * (pixel_delta_u + pixel_delta_v);
        }

        ray get_ray(int i, int j) const {
            auto offset = sample_square();
            auto pixel_sample = origin + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

            auto ray_origin = center;
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 sample_square() const {
            return vec3(random_double() - .5, random_double() - .5, 0);
        }

        color ray_color(const ray& r, const hittable& world) const {
            hit_info info;
            if (world.hit(r, interval(0, infinity), info)) {
                vec3 direction = random_on_hemisphere(info.normal);
                return .5 * ray_color(ray(info.p, direction), world);
            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = .5*(unit_direction.y() + 1.0);
            return (1.0 - a) * color(1.0, 1.0, 1.0) + a*color(0.5, .7, 1.0);
        }


};



#endif
