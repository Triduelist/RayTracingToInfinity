//
// Created by Andrew Yang on 3/27/21.
//

#include <iostream>
#include <thread>
#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "scenes.hpp"
#include "raytracer.hpp"
#include "timer.hpp"
#include "parallel/pixels.hpp"
#include "parallel/task.hpp"
#include "parallel/params.hpp"
#include "hittable/rectangles.hpp"
#include "denoise.hpp"

float params::ASPECT_RATIO = 1.0f;
unsigned params::WIDTH = 200;
unsigned params::HEIGHT = int(params::WIDTH / params::ASPECT_RATIO);

unsigned params::N = 16;//16;
unsigned params::N_samples = 20;
unsigned params::MAX_DEPTH = 50;

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
        case 1:
            world = random_scene();
            background = color(.7f, .8f, 1.0f);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0f;
            aperture = .1f;
            break;
        default:
        case 2:
            world = mesh_test();
            lookfrom = point3(13, 6, 3);
            lookat = point3(0,1,0);
            vfov = 20.0f;
            background = color(.7f, .8f, 1.0f);
            aperture = .1f;
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

    // Render window

    sf::RenderWindow window(sf::VideoMode(params::WIDTH, (params::WIDTH/params::ASPECT_RATIO)),
                            "Ray Tracing", sf::Style::Titlebar | sf::Style::Close);
    window.setSize(sf::Vector2u(700, 700/params::ASPECT_RATIO));

    sf::Texture tex;
    sf::Sprite sprite;

    if(!tex.create(params::WIDTH, params::HEIGHT)) {
        std::cerr << "Couldn't create texture!" << std::endl;
        return 1;
    }

    tex.setSmooth(false);

    sprite.setTexture(tex);

    pixels pix = pixels(params::WIDTH, params::HEIGHT);
    std::vector<float> data(params::WIDTH * params::HEIGHT * 5);

    // Camera

    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0f;
    camera cam(lookfrom, lookat, vup, vfov, params::ASPECT_RATIO, aperture, dist_to_focus, .0f, 1.0f);

    const unsigned int n_threads = std::thread::hardware_concurrency();
    std::cout << "Detected " << n_threads << " concurrent threads." << std::endl;
    std::vector<std::thread> threads(n_threads);

    Timer timer;

    for(auto& t : threads) t = std::thread(Task{&world, &cam, background, &data[0]});

    bool finished_rendering = false;

    while(window.isOpen()) {
        sf::Event event{};
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) window.close();
        }

        tex.update(&pix.get_pixels(data)[0]);

        window.clear();
        window.draw(sprite);
        window.display();

        if(!finished_rendering && done_count == n_threads) {
            std::cout << "Finished rendering in " << timer.get_millis() << " ms or "
                      << timer.get_seconds() << "s. " << std::endl;
            std::string option{};
            std::cout << "Apply denoise? (Y or N): ";
            std::cin >> option;
            if(option == "Y" || option == "y" || option == "yes" || option == "Yes") {
                data_denoise(data);
                std::cout << "Denoise applied successfully!";
            }
            std::cout << "No denoise applied.";
            finished_rendering = true;
        }

        sf::sleep(sf::milliseconds(1000));
    }

    std::cout << "Waiting for all threads to join" << std::endl;
    for(auto& t : threads) t.join();

    tex.copyToImage().saveToFile("output/out.png");
    std::cout << "Saved image to out.png" << std::endl;

    return 0;
}