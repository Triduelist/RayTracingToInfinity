//
// Created by Andrew Yang on 3/27/21.
//

#ifndef RAYTRACING_TIMER_H
#define RAYTRACING_TIMER_H

#include <chrono>

class Timer {
public:
    Timer() : start{std::chrono::high_resolution_clock::now()} {}

    [[nodiscard]] unsigned get_millis() const {
        const auto elapsed = std::chrono::high_resolution_clock::now() - start;
        return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    }

    [[nodiscard]] unsigned get_seconds() const {
        const auto elapsed = std::chrono::high_resolution_clock::now() - start;
        return std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
    }

    [[nodiscard]] std::string to_string() const {
        int n = (int)get_seconds();
        int h = n / 3600;
        n %= 3600;
        int m = n / 60;
        n %= 60;
        return std::to_string(h) + " hours, " + std::to_string(m) + " minutes, " + std::to_string(n) + " seconds.\n";
    }

private:
    std::chrono::high_resolution_clock::time_point start;
};

#endif //RAYTRACING_TIMER_H
