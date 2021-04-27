//
// Created by Andrew Yang on 3/27/21.
//

#include <thread>

#include "scenes.hpp"
#include "raytracer.hpp"
#include "parallel/params.hpp"
#include "hittable/rectangles.hpp"
#include "render.hpp"

float params::ASPECT_RATIO = 16.f/9.f;
unsigned params::WIDTH = 200;
unsigned params::HEIGHT = int(params::WIDTH / params::ASPECT_RATIO);

unsigned params::N = 16;//16;
unsigned params::N_samples = 10;
unsigned params::MAX_DEPTH = 16;//50

unsigned params::W_CNT = (params::WIDTH + params::N - 1) / params::N;
unsigned params::H_CNT = (params::HEIGHT + params::N - 1) / params::N;

int main() {
    hittable_list world;

    point3 lookfrom;
    point3 lookat;
    auto vfov = 40.0f;
    auto aperture = .0f;
    color background(0, 0, 0);
    auto time0 = .0f;
    auto time1 = 1.0f;
    switch(0) {
        default:
        case 1:
            world = random_scene();
            background = color(.7f, .8f, 1.0f);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0f;
            aperture = .1f;
            break;
        case 2:
            world = gold_coin();
            lookfrom = point3(0, 4, -3);
            lookat = point3(0,0,0);
            params::ASPECT_RATIO = 16.f/9.f;
            params::HEIGHT = int(params::WIDTH / params::ASPECT_RATIO);
            break;
        case 3:
            world = mesh_test();
            lookfrom = point3(0, 1, -7);
            lookat = point3(0,1,0);
            background = color(.7f, .8f, 1.0f);
            params::ASPECT_RATIO = 16.f/9.f;
            params::HEIGHT = int(params::WIDTH / params::ASPECT_RATIO);
            break;
        case 4:
            world = moving_spheres();
            lookfrom = point3(378, 300, 50);
            lookat = point3(-150, -150, -150);
            params::ASPECT_RATIO = 3.0f/2.0f;
            params::HEIGHT = int(params::WIDTH / params::ASPECT_RATIO);
            break;
        case 6:
            world = cornell_box();
            background = color(0, 0, 0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0f;
            break;
        case 7:
            world = cornell_smoke();
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0f;
            break;
        case 8:
            world = final_scene();
            background = color(0,0,0);
            lookfrom = point3(478, 278, -600);
            lookat = point3(278, 278, 0);
            vfov = 40.0f;
            break;
        case 9:
            world = single_cylinder();
            background = color(.7f, .8f, 1.0f);
            lookfrom = point3(0, 2, -15);
            lookat = point3(0, 0, 0);
            vfov = 20.0f;
            aperture = .1f;
            break;
        case 10:
            params::N_samples = 10;
            world = single_cone();
            background = color(.7f, .8f, 1.0f);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0f;
            aperture = .1f;
            break;
        case 11:
            world = mapped_box();
            background = color(1,1,1);
            lookfrom = point3(9, -1, 0);
            lookat = point3(0, -1, 0);
            vfov = 40.0f;
            aperture = .02f;
            break;
        case 12:
            world = cornell_glass();
            background = color(0, 0, 0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0f;
            break;
    }

    render_window(lookfrom, lookat, vfov, aperture, world, background);

    return 0;
}