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
#include "rtweekend.hpp"

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

    static void get_cylinder_uv(const point3& p, float& u, float& v) {
        // Compute the azimutal angle
        float theta = std::atan2(p.x(), p.z());
        float raw_u = theta / (2 * fpi);
        u = 1 - (raw_u + .5f);
        v = p.y(); // - floor(p.x());
    }
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
        point3 p = r.origin() + r.direction() * x;

        if(x == t1) {
            if(std::abs(p.z() - center.z()) < height / 2.0f) {
                outward_normal = vec3(p.x() - center.x(), p.y() - center.y(), 0);
                outward_normal = unit_vector(outward_normal);
                t = x;
                break;
            }
        }
        else {
            if(std::pow(p.x() - center.x(), 2) + std::pow(p.y() - center.y(), 2) - std::pow(radius, 2) <= 0) {
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
    get_cylinder_uv(rec.normal, rec.u, rec.v);

    return true;
}

bool cylinder::bounding_box(float time0, float time1, aabb &output_box) const {
    output_box = aabb(center - point3(radius, height/2, radius), center + point3(radius, height/2, radius));
    return true;
}

#endif //RAYTRACING_CYLINDER_HPP
