//
// Created by Andrew Yang on 4/5/21.
//

#ifndef RAYTRACING_CONE_HPP
#define RAYTRACING_CONE_HPP

#include <utility>
#include <vector>

#include "hittable.hpp"
#include "aabb.hpp"
#include "vec3.hpp"

class cone : public hittable {
public:
    cone(point3 cen, float r, float h, float tz, shared_ptr<material> m)
        : center(cen), radius(r), height(h), tipz(tz), mat_ptr(std::move(m))
    {}

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

    bool bounding_box(float time0, float time1, aabb& output_box) const override;

private:
    point3 center;
    float radius, height, tipz;
    shared_ptr<material> mat_ptr;
};

bool cone::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    vec3 eminusc = r.origin() - center;

    float H = tipz - center.z();
    float R = radius;
    float s = std::powf(R, 2) / std::powf(H, 2);

    float a = std::powf(r.direction().x(), 2) + std::powf(r.direction().y(), 2) - s * std::powf(r.direction().z(), 2);
    float b = 2 * (r.direction().x() * eminusc.x() + r.direction().y()
            * eminusc.y() - s * (eminusc.z()-H) * r.direction().z());
    float c = std::powf(eminusc.x(), 2) + std::powf(eminusc.y(), 2) - s * std::powf(eminusc.z() - H, 2);
    float discriminant = b * b - 4 * a * c;
    if(discriminant < 0)
        return false;

    float t1a = (-b + std::sqrtf(discriminant)) / (2*a);
    float t1b = (-b - std::sqrtf(discriminant)) / (2*a);
    float t2 = (height / 2.0f - eminusc.z()) / r.direction().z();
    float t3 = (-height / 2.0f - eminusc.z()) / r.direction().z();

    float tarr[] = {t1a, t1b, t2, t3};
    std::sort(std::begin(tarr), std::end(tarr));

    float t = INT_MIN;
    vec3 outward_normal;
    for(float x : tarr) {
        point3 p = r.origin() + r.direction() * x;

        if(x == t1a || x == t1b) {
            if(std::abs(p.z() - center.z()) < height / 2) {
                outward_normal = vec3(p.x() - center.x(), p.y() - center.y(), 0);
                outward_normal = unit_vector(outward_normal);
                outward_normal = outward_normal + vec3(0, 0, R / H);
                outward_normal = unit_vector(outward_normal);
                t = x;
                break;
            }
        }
        else if(x == t2) {
            if(std::powf(p.x() - center.x(), 2)
            + std::powf(p.y() - center.y(), 2)
            - std::powf((R / H) * (H - height / 2), 2) <= 0) {
                outward_normal = vec3(0, 0, 1);
                t = x;
                break;
            }
        }
        else if(x == t3) {
            if(std::powf(p.x() - center.x(), 2)
               + std::powf(p.y() - center.y(), 2)
               - std::powf((R / H) * (H + height / 2), 2) <= 0) {
                outward_normal = vec3(0, 0, -1);
                t = x;
                break;
            }
        }
    }

    if(t == INT_MIN || t > t_max || t < t_min)
        return false;
    rec.mat_ptr = mat_ptr;
    rec.t = t;
    rec.p = r.at(t);
    rec.set_face_normal(r, outward_normal);

    return true;
}

bool cone::bounding_box(float time0, float time1, aabb &output_box) const {
    return false;
}

#endif //RAYTRACING_CONE_HPP
