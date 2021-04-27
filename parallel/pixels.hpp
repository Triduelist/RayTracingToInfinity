//
// Created by Andrew Yang on 3/28/21.
//

#ifndef RAYTRACING_PIXELS_HPP
#define RAYTRACING_PIXELS_HPP

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

#include "rtweekend.hpp"

class pixels {
public:
    pixels() = default;
    pixels(unsigned w, unsigned h)
            : width{w}, height{h},
              pix(width * height * 4) // RGBA
    {}

    std::vector<sf::Uint8> get_pixels(std::vector<float>& data) {
        // Convert accumulated pixels so we can display them
        for(int i=0; i < height; ++i) {
            for(int j=0; j < width; ++j) {
                const unsigned data_pos = (i * width + j) * 5;
                const unsigned pix_pos = ((height - i - 1) * width + j) << 2;
                const float ns = data[data_pos + 4]; // number of accumulated values
//                // Replace nan components with zero
//                if(data[data_pos + 0] != data[data_pos + 0]) data[data_pos + 0] = .0f;
//                if(data[data_pos + 1] != data[data_pos + 1]) data[data_pos + 1] = .0f;
//                if(data[data_pos + 2] != data[data_pos + 2]) data[data_pos + 2] = .0f;

                pix[pix_pos + 0] = sf::Uint8(colCap(255.99 * sqrt(data[data_pos + 0] / ns)));
                pix[pix_pos + 1] = sf::Uint8(colCap(255.99 * sqrt(data[data_pos + 1] / ns)));
                pix[pix_pos + 2] = sf::Uint8(colCap(255.99 * sqrt(data[data_pos + 2] / ns)));
                pix[pix_pos + 3] = 255u;
            }
        }
        return pix;
    }

private:
    unsigned width{};
    unsigned height{};
    std::vector<sf::Uint8> pix{}; // RGBA
};

#endif //RAYTRACING_PIXELS_HPP
