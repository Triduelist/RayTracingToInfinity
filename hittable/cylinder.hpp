//
// Created by Andrew Yang on 4/5/21.
//

#ifndef RAYTRACING_CYLINDER_HPP
#define RAYTRACING_CYLINDER_HPP

#include <utility>
#include <vector>

#include "hittable.hpp"
#include "aabb.hpp"
#include "vec3.hpp"

class cylinder : public hittable {
public:
    cylinder(point3 cen, float r, float h, shared_ptr<material> m)
        : center(cen), radius(r), height(h), mat_ptr(std::move(m))
    {}

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

    bool bounding_box(float time0, float time1, aabb& output_box) const override;

private:
    point3 center;
    float radius, height;
    shared_ptr<material> mat_ptr;
};

bool cylinder::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 eminusc = r.origin() - center;

    float a = (std::powf(r.direction().x(), 2)) + (std::powf(r.direction().y(), 2));
    float b = 2*(r.direction().x() * eminusc.x() + r.direction().y() * eminusc.y());
    float c = std::powf(eminusc.x(), 2) + std::powf(eminusc.y(), 2) - std::powf(radius, 2);
    float discriminant = b * b - 4 * a * c;
    if(discriminant < 0)
        return false;

    float t1 = std::min((-b+std::sqrt(discriminant))/(2*a), (-b-std::sqrt(discriminant))/(2*a));
    float t2 = (height / 2.0f - eminusc.z()) / r.direction().z();
    float t3 = (-height / 2.0f - eminusc.z()) / r.direction().z();

    // We'll iterate through the values in sorted order so we find closest intersection first
    float tarr[] = {t1, t2, t3};
    //std::vector<float> tarr{t1, t2, t3};
    std::sort(std::begin(tarr), std::end(tarr));

    float t = INT_MIN;
    vec3 outward_normal{};
    for(float x : tarr) {
        hit_record tmp;
        tmp.p = r.origin() + r.direction() * x;

        if(x == t1) {
            if(std::abs(tmp.p.z() - center.z()) < height / 2.0f) {
                outward_normal = vec3(tmp.p.x() - center.x(), tmp.p.y() - center.y(), 0);
                outward_normal = unit_vector(outward_normal);
                t = x;
                break;
            }
        }
        else {
            if(std::pow(tmp.p.x() - center.x(), 2) + std::pow(tmp.p.y() - center.y(), 2) - std::pow(radius, 2) <= 0) {
                if(x == t2) {
                    outward_normal = vec3(0,0,1);
                }
                else if(x == t3) {
                    outward_normal = vec3(0,0,-1);
                }
                t = x;
                break;
            }
        }
    }

    if(t == INT_MIN || t > t_max || t < t_min) {
        return false;
    }
    rec.mat_ptr = mat_ptr;
    rec.t = t;
    rec.p = r.at(t);
    rec.set_face_normal(r, outward_normal);

    return true;
}

bool cylinder::bounding_box(float time0, float time1, aabb &output_box) const {
    output_box = aabb(center - point3(radius, radius, height), center + point3(radius, radius, height));
    return true;
}

#endif //RAYTRACING_CYLINDER_HPP
