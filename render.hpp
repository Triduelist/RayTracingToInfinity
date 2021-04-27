//
// Created by Andrew Yang on 4/24/21.
//

#ifndef RAYTRACING_RENDER_HPP
#define RAYTRACING_RENDER_HPP

#include <SFML/Graphics.hpp>

#include "denoise.hpp"
#include "timer.hpp"
#include "parallel/pixels.hpp"
#include "parallel/task.hpp"
#include "parallel/params.hpp"

void render_window(point3& lookfrom, point3& lookat, float vfov, float aperture, hittable_list& world, color& background) {
    // Render window

    sf::RenderWindow window(sf::VideoMode(params::WIDTH, (params::WIDTH/params::ASPECT_RATIO)),
                            "Ray Tracing", sf::Style::Titlebar | sf::Style::Close);
    window.setSize(sf::Vector2u(700, 700/params::ASPECT_RATIO));

    sf::Texture tex;
    sf::Sprite sprite;

    if(!tex.create(params::WIDTH, params::HEIGHT)) {
        std::cerr << "Couldn't create texture!" << std::endl;
        return;
    }

    tex.setSmooth(false);

    sprite.setTexture(tex);

    pixels pix = pixels(params::WIDTH, params::HEIGHT);
    std::vector<float> data(params::WIDTH * params::HEIGHT * 5);

    int wind_w = 700;
    int wind_h = 700/params::ASPECT_RATIO;
    float popw = wind_w / 2.0f;
    float poph = wind_h / 2.5f;
    sf::RectangleShape popup(sf::Vector2f(popw, poph));
    popup.move((float)wind_w/2 - popw/2, (float)wind_h/2 - poph/2);
    popup.setOutlineThickness(7);
    popup.setOutlineColor(sf::Color(120, 120, 120));

    sf::Font title;
    title.loadFromFile("/Users/andrewyang/CLionProjects/untitled/arial.ttf");
    sf::Text textt("Title", title);
    textt.setFillColor(sf::Color::Black);
    textt.setCharacterSize(wind_w / 43.333f);
    sf::FloatRect textTitle = textt.getLocalBounds();
    textt.setOrigin(textTitle.left + textTitle.width/2.0f,
                    textTitle.top  + textTitle.height/2.0f);
    textt.setPosition(sf::Vector2f((float)wind_w/2.7f,(float)wind_h/2.7f));

    sf::RectangleShape buttonl(sf::Vector2f(wind_w/9, wind_h/11.5f));
    buttonl.setFillColor(sf::Color::Blue);
    buttonl.move((float)wind_w/2 - wind_w/4.4, (float)wind_h/1.75f);

    sf::Font fontl;
    fontl.loadFromFile("/Users/andrewyang/CLionProjects/untitled/arial.ttf");
    sf::Text textl("No", fontl);
    textl.setFillColor(sf::Color::White);
    textl.setCharacterSize(wind_w / 43.333f);
    sf::FloatRect textRectl = textl.getLocalBounds();
    textl.setOrigin(textRectl.left + textRectl.width/2.0f,
                    textRectl.top  + textRectl.height/2.0f);
    textl.setPosition(sf::Vector2f((float)wind_w/3.05f,(float)wind_h/1.62f));

    sf::RectangleShape buttonr(sf::Vector2f(wind_w/9, wind_h/11.5f));
    buttonr.setFillColor(sf::Color::Blue);
    buttonr.move((float)wind_w/2 + (float)wind_w/9, (float)wind_h/1.75f);

    sf::Font fontr;
    fontr.loadFromFile("/Users/andrewyang/CLionProjects/untitled/arial.ttf");
    sf::Text textr("Yes", fontr);
    textr.setFillColor(sf::Color::White);
    textr.setCharacterSize(wind_w / 43.333f);
    sf::FloatRect textRectr = textl.getLocalBounds();
    textr.setOrigin(textRectr.left + textRectr.width/2.0f,
                    textRectr.top  + textRectr.height/2.0f);
    textr.setPosition(sf::Vector2f((float)wind_w/1.52f,(float)wind_h/1.62f));

    bool clicked = false;
    bool hide = false;

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
            if(finished_rendering && !clicked) {
                if(event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if (buttonl.getGlobalBounds().contains(mousePos)) {
                        buttonl.setFillColor(sf::Color(20,20,150));
                    }
                    if (buttonr.getGlobalBounds().contains(mousePos)) {
                        buttonr.setFillColor(sf::Color(20,20,150));
                    }
                }
                else if(event.type == sf::Event::MouseButtonReleased) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if(buttonl.getGlobalBounds().contains(mousePos)) {
                        window.create(sf::VideoMode(params::WIDTH, (params::WIDTH/params::ASPECT_RATIO)),
                                      "Ray Tracing", sf::Style::Titlebar | sf::Style::Close);
                        window.setSize(sf::Vector2u(700, 700/params::ASPECT_RATIO));
                        clicked = true;
                        hide = false;
                    }
                    if(buttonr.getGlobalBounds().contains(mousePos)) {
                        window.create(sf::VideoMode(params::WIDTH, (params::WIDTH/params::ASPECT_RATIO)),
                                      "Ray Tracing", sf::Style::Titlebar | sf::Style::Close);
                        window.setSize(sf::Vector2u(700, 700/params::ASPECT_RATIO));
                        data_denoise(data);
                        tex.update(&pix.get_pixels(data)[0]);
                        clicked = true;
                        hide = false;
                    }
                    buttonl.setFillColor(sf::Color::Blue);
                    buttonr.setFillColor(sf::Color::Blue);
                }
            }
        }

        if(!finished_rendering)
            tex.update(&pix.get_pixels(data)[0]);

        window.clear();
        if(!hide)
            window.draw(sprite);
        if(!clicked && finished_rendering) {
            window.draw(popup);
            window.draw(buttonl);
            window.draw(textl);
            window.draw(buttonr);
            window.draw(textr);
            window.draw(textt);
        }
        window.display();

        if(!finished_rendering && done_count == n_threads) {
            textt.setString("        Finished rendering in:\n" + timer.to_string() + "  Apply OpenImageDenoise?");
            window.create(sf::VideoMode(wind_w, wind_h),
                          "Ray Tracing", sf::Style::Titlebar | sf::Style::Close);
            hide = true;
            finished_rendering = true;
        }

        sf::sleep(sf::milliseconds(200));
    }

    std::cout << "Waiting for all threads to join" << std::endl;
    for(auto& t : threads) t.join();

    tex.copyToImage().saveToFile("output/out.png");
    std::cout << "Saved image to out.png" << std::endl;
    return;
}

#endif //RAYTRACING_RENDER_HPP
