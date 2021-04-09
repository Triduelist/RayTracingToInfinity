//
// Created by Andrew Yang on 3/22/21.
//

#ifndef RAYTRACING_RTWEEKEND_HPP
#define RAYTRACING_RTWEEKEND_HPP

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>
#include <random>


// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const float f_infinity = std::numeric_limits<float>::infinity();
const double pi = 3.1415926535897932385;
const float fpi = 3.1415926535897932385f;

// Utility functions

inline float degrees_to_radians(float degrees) {
    return degrees * fpi / 180.0;
}

inline double random_double() {
    // Returns a random real between [0, 1)
    static std::uniform_real_distribution<double> distribution(.0, 1.0);
    std::random_device dev;
    static std::minstd_rand generator(dev());
    return distribution(generator);
}

inline double random_double(double min, double max) {
    // returns a random real between [min, max)
    return min + (max-min)*random_double();
}

inline float random_float() {
    // Returns a random float real between [0, 1)
    static std::uniform_real_distribution<float> distribution(.0, 1.0);
    std::random_device dev;
    static std::minstd_rand generator(dev());
    return distribution(generator);
}

inline float random_float(float min, float max) {
    // returns a random float real between [min, max)
    return min + (max-min)*random_float();
}

inline double clamp(double x, double min, double max) {
    if(x < min) return min;
    if(x > max) return max;
    return x;
}

inline float fclamp(float x, float min, float max) {
    if(x < min) return min;
    if(x > max) return max;
    return x;
}

inline int random_int(int min, int max) {
    // Returns a random integer in [min, max].
    return static_cast<int>(random_float(min, max));
}

inline int colCap(int x) {
    return x > 255 ? 255 : x;
}

// Common headers

#include "ray.hpp"
#include "vec3.hpp"

#endif //RAYTRACING_RTWEEKEND_HPP
