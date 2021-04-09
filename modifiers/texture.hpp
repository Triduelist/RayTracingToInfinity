//
// Created by Andrew Yang on 3/31/21.
//

#ifndef RAYTRACING_TEXTURE_HPP
#define RAYTRACING_TEXTURE_HPP

#include <utility>

#include "rtweekend.hpp"
#include "perlin.hpp"
#include "rtw_stb_image.hpp"

class texture {
public:
    [[nodiscard]] virtual color value(float u, float v, const point3& p) const = 0;
};

class solid_color : public texture {
public:
    solid_color() = default;
    explicit solid_color(color c) : color_value(c) {}

    solid_color(float red, float green, float blue)
            : solid_color(color(red,green,blue)) {}

    color value(float u, float v, const point3& p) const override {
        return color_value;
    }

private:
    color color_value;
};

class checker_texture : public texture {
public:
    checker_texture() = default;

    checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd)
        : even(std::move(_even)), odd(std::move(_odd))
    {}

    checker_texture(color c1, color c2)
        : even(make_shared<solid_color>(c1)), odd(make_shared<solid_color>(c2))
    {}

    [[nodiscard]] color value(float u, float v, const point3& p) const override {
        auto sines = sin(10*p.x()) * sin(10*p.y()) * sin(10*p.z());
        if(sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }

public:
    shared_ptr<texture> odd;
    shared_ptr<texture> even;
};

class noise_texture : public texture {
public:
    noise_texture() = default;
    explicit noise_texture(float sc) : scale(sc) {}

    [[nodiscard]] color value(float u, float v, const point3& p) const override {
        return color(1, 1, 1) * .5f * (1 + sin(scale*p.z() + 10*noise.turb(p)));
    }

public:
    perlin noise;
    float scale{};
};

class corner_texture : public texture {
public:
    corner_texture() = default;

    [[nodiscard]] color value(float u, float v, const point3& p) const override {
        if(v > .8f) {
            if(u < .2f) return color(1,0,0);
            if(u > .8f) return color(1,1,0);
        }
        else if(v < .2f) {
            if(u < .2f) return color(0,1,0);
            if(u > .8f) return color(0,1,1);
        }
        return color(1,1,1);
    }
};

class image_texture : public texture {
public:
    const static int bytes_per_pixel = 3;

    image_texture()
        : data(nullptr), width(0), height(0), bytes_per_scanline(0)
    {}

    image_texture(const char* filename) {
        auto components_per_pixel = bytes_per_pixel;

        data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

        if(!data) {
            std::cerr << "ERROR: Could not load texture image file " << filename << std::endl;
            width = height = 0;
        }

        bytes_per_scanline = bytes_per_pixel * width;
    }

    ~image_texture() {
        delete data;
    }

    [[nodiscard]] color value(float u, float v, const vec3& p) const override {
        // If we have no texture data, then return a solid cyan as a debugging aid
        if(data == nullptr)
            return color(0,1,1);

        // fclamp imput texture coordiantes to [0,1] x [1,0]
        u = fclamp(u, .0f, 1.0f);
        v = 1.0f - fclamp(v, .0f, 1.0f); // Flip V to image coordinates

        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);

        // fclamp integer mapping, since actual coordinates should be less than 1.0
        if(i >= width) i = width-1;
        if(j >= height) j = height-1;

        const auto color_scale = 1.0f / 255.0f;
        auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

        return color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
    }

private:
    unsigned char *data;
    int width, height;
    int bytes_per_scanline;
};

#endif //RAYTRACING_TEXTURE_HPP
