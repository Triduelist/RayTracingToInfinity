//
// Created by Andrew Yang on 3/30/21.
//

#ifndef RAYTRACING_AABB_HPP
#define RAYTRACING_AABB_HPP

#include "rtweekend.hpp"

class aabb {
public:
    aabb() = default;
    aabb(const point3& a, const point3& b) { minimum = a; maximum = b; }

    [[nodiscard]] point3 min() const { return minimum; }
    [[nodiscard]] point3 max() const { return maximum; }

    inline bool hit(const ray& r, float& t_min, float& t_max) const {
        for (int a = 0; a < 3; a++) {
            auto invD = 1.0f / r.direction()[a];
            auto t0 = (min()[a] - r.origin()[a]) * invD;
            auto t1 = (max()[a] - r.origin()[a]) * invD;
            if (invD < 0.0f)
                std::swap(t0, t1);
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            if (t_max <= t_min)
                return false;
        }
        return true;
    }

    int get_longest_axis() {
        vec3 diff = minimum - maximum;
        if(diff.x() > diff.y() && diff.x() > diff.z()) return 0;
        if(diff.y() > diff.x() && diff.y() > diff.z()) return 1;
        return 2;
    }

private:
    point3 minimum;
    point3 maximum;
};

aabb surrounding_box(aabb box0, aabb box1) {
    point3 small(fmin(box0.min().x(), box1.min().x()),
                 fmin(box0.min().y(), box1.min().y()),
                 fmin(box0.min().z(), box1.min().z()));
    point3 big(fmax(box0.max().x(), box1.max().x()),
               fmax(box0.max().y(), box1.max().y()),
               fmax(box0.max().z(), box1.max().z()));
    return aabb(small, big);
}

#endif //RAYTRACING_AABB_HPP
