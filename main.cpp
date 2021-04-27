#include "rtweekend.hpp"
#include "camera.hpp"

#include "color.hpp"
#include "hittable/hittable_list.hpp"
#include "hittable/sphere.hpp"
#include "modifiers/material.hpp"
#include "timer.hpp"
#include "raytracer.hpp"
#include "scenes.hpp"

#include <iostream>

int main() {
    // Image
//    const auto aspect_ratio = 3.0 / 2.0;
    const auto aspect_ratio = 16.0/9.0;
    const int image_width = 500;
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    const int samples_per_pixel = 10;
    const int max_depth = 16;

    // World
    auto world = mesh_test();

    // Camera

//    point3 lookfrom(13, 2, 3);
    color background = color(0,0,0);
    point3 lookfrom = point3(0, 4, -3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = .1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Render
    Timer timer;
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for(int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
        for(int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for(int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_float()) / (image_width-1);
                auto v = (j + random_float()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color2(r, background, &world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nFinished rendering in " << timer.get_millis() << " ms or "
              << timer.get_seconds() << "s. ";
    std::cerr << "\nDone\n";
    return 0;
}
