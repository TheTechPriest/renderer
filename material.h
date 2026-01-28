#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material {

    public:
        virtual ~material() = default;

        virtual bool scatter(
            [[maybe_unused]] const ray& r_in,  [[maybe_unused]] const hit_info& info,  [[maybe_unused]] color& attenuation,  [[maybe_unused]] ray& scattered
        ) const {return false;}

};

class lambertian : public material {
    public:
        lambertian(const color& albedo) : albedo(albedo) {}

        bool scatter([[maybe_unused]] const ray& r_in, const hit_info& info, color& attenuation, ray& scattered)
        const override {
            auto scatter_direction = info.normal + random_unit_vector();

            if (scatter_direction.near_zero())
                scatter_direction = info.normal;
            scattered = ray(info.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
    private:
        color albedo;
};

class metal : public material {
    public:
        metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}
        bool scatter(const ray& r_in, const hit_info& info, color& attenuation, ray& scattered)
        const override {
            vec3 reflected = reflect(r_in.direction(), info.normal);
            reflected = unit_vector(reflected) + (fuzz *random_unit_vector());
            scattered = ray(info.p, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), info.normal) > 0);
        }

    private:
        color albedo;
        double fuzz;
};

class dielectric : public material {
    public:
        dielectric(double refraction_index) : refraction_index(refraction_index) {}

        bool scatter (const ray& r_in, const hit_info& info, color& attenuation, ray& scattered)
        const override {
            attenuation = color(1.0, 1.0, 1.0);
            double ri = info.front_face ? (1.0 / refraction_index) : refraction_index;

            vec3 unit_direction = unit_vector(r_in.direction());
            double cos_theta = std::fmin(dot(-unit_direction, info.normal), 1.0);
            double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = ri * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refract || reflectance(cos_theta, ri) > random_double())
                direction = reflect(unit_direction, info.normal);
            else
                direction = refract(unit_direction, info.normal, ri);

            scattered = ray(info.p, direction);
            return true;
        }

    private:
        double refraction_index;

        static double reflectance(double cosine, double refraction_index) {
            auto r0 = (1 - refraction_index) / (1 + refraction_index);
            r0 = r0*r0;
            return r0 + (1-r0)* std::pow((1-cosine), 5);
        }


};

#endif
