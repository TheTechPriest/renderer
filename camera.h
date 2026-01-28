#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera {

    public:
        double aspect_ratio = 1.0;
        int imageW = 100;
        // Samples per pixel - Antialiasing setting
        int spp = 10;
        int max_depth = 10;

        double vfov = 90;
        point3 lookfrom = point3(0,0,0);
        point3 lookat = point3(0,0, -1);
        vec3 vup = vec3(0, 1, 0);

        double defocus_angle = 0;
        double focus_dist = 10;

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
                        pixel_color += ray_color(r, max_depth, world);
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
        vec3 u, v, w;
        vec3 defocus_disk_u;
        vec3 defocus_disk_v;

        void initialize() {
            imageH = int(imageW / aspect_ratio);
            imageH = (imageH < 1) ? 1 : imageH;

            pixel_samples_scale = 1.0 / spp;

            center = lookfrom;

//            auto focal_length = (lookfrom - lookat).length();
            auto theta = degrees_to_radians(vfov);
            auto h = std::tan(theta/2);
            auto viewportH = 2 * h * focus_dist;
            auto viewportW = viewportH * (double(imageW) / imageH);

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            vec3 viewport_u = viewportW * u;
            vec3 viewport_v = viewportH * -v;

            pixel_delta_u = viewport_u / imageW;
            pixel_delta_v = viewport_v / imageH;

            auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
            origin = viewport_upper_left + .5 * (pixel_delta_u + pixel_delta_v);

            auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;
        }

        ray get_ray(int i, int j) const {
            auto offset = sample_square();
            auto pixel_sample = origin + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

            auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 sample_square() const {
            return vec3(random_double() - .5, random_double() - .5, 0);
        }

        point3 defocus_disk_sample() const {
            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }

        color ray_color(const ray& r, int depth, const hittable& world) const {
            if (depth <= 0)
                return color(0, 0, 0);
            hit_info info;
            if (world.hit(r, interval(0.001, infinity), info)) {
                ray scattered;
                color attenuation;
                if (info.mat->scatter(r, info, attenuation, scattered))
                    return attenuation * ray_color(scattered, depth-1, world);
                return color(0,0,0);
                // vec3 direction = info.normal + random_unit_vector();
                // return .5 * ray_color(ray(info.p, direction), depth-1, world);
            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = .5*(unit_direction.y() + 1.0);
            return (1.0 - a) * color(1.0, 1.0, 1.0) + a*color(0.5, .7, 1.0);
        }


};



#endif
