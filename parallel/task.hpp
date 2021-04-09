//
// Created by Andrew Yang on 3/28/21.
//

#ifndef RAYTRACING_TASK_HPP
#define RAYTRACING_TASK_HPP

#include <atomic>
#include <vector>

#include "params.hpp"

std::atomic<unsigned> done_count;

class Task {
public:
    Task(hittable_list& w, camera& c, color& back, float* d)
            : my_id{id++}, world{make_shared<hittable_list>(w)},
              cam{make_shared<camera>(c)}, background(back), data{d}
    {}

    static void move_in_pattern(int& rx, int& ry) {
        // snake pattern implementation
        static int x = -1, y = int(params::H_CNT) - 1;
        static int dir = 0;

        x = dir ? x - 1 : x + 1;
        if(x == params::W_CNT || x == -1) {
            x = y & 1 ? int(params::W_CNT) - 1 : 0;
            y--;
            dir = !dir;
        }
        rx = x;
        ry = y;
    }

    bool get_next_task() {
        static std::vector<std::vector<bool>> taken(params::H_CNT, std::vector<bool>(params::W_CNT));
        static std::mutex m;

        std::lock_guard<std::mutex> guard{m};

        bool found = false;
        int x, y;
        while(!found) {
            move_in_pattern(x, y);
            if(x < 0 || x > params::W_CNT || y < 0 || y > params::H_CNT) break;

            if(!taken[y][x]) {
                sx = x * int(params::N);
                sy = y * int(params::N);
                taken[y][x] = true;
                found = true;
            }
        }

        return found;
    }

    void operator()() {
        bool done = false;
        do {
            if(!get_next_task()) {
                done = true;
                continue;
            }

            for(unsigned s=0; s < params::N_samples; ++s) {
                for(unsigned y=sy; y < sy + params::N; ++y) {
                    for(unsigned x=sx; x < sx + params::N; ++x) {
                        if(x < 0 || y < 0 || x >= params::WIDTH || y >= params::HEIGHT) continue;

                        const auto u = (float)((x + random_float()) / (params::WIDTH));
                        const auto v = (float)((y + random_float()) / (params::HEIGHT));
                        ray r = cam->get_ray(u, v);
                        const vec3 col = ray_color2(r, background, world, params::MAX_DEPTH);
                        const unsigned pos = (y * params::WIDTH + x) * 5;
                        data[pos + 0] += col.x();
                        data[pos + 1] += col.y();
                        data[pos + 2] += col.z();
                        data[pos + 3] += 255; // opaque
                        data[pos + 4] += 1; // number of samples
                    }
                }
            }
        } while(!done);

        done_count++;

        std::cout << "Thread " << my_id << " is done!" << std::endl;
    }

    int sx = -1, sy = -1;
    int my_id;
    static int id;
    shared_ptr<hittable_list> world{};
    shared_ptr<camera> cam{};
    float* data;
    color background{};
};

int Task::id = 0;

#endif //RAYTRACING_TASK_HPP
