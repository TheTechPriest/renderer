#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"
//#include <iostream>

// Creating a version of our vector class called 'color' containing a pixel's color info

using color = vec3;

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Turning the normalized vector coords into its rgb value equivalent (0-1 to 0-256)
    static const interval intensity(0.000, 9.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
 

#endif
