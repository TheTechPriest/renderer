#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>

int main() {

    int imageW = 256;
    int imageH = 256;

    std::cout << "P3\n" << imageW << ' ' << imageH << "\n255\n";

    for (int j = 0; j < imageH; j++) {
        std::clog << "\rScanlines remaining: " << (imageH - j) << ' ' << std::flush;
        for (int i = 0; i < imageW; i++) {
            auto pixel_color = color(double(i) / (imageW-1), double(j) / (imageH-1), 0);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";

}
