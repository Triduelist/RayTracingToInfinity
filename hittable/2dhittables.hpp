//
// Created by Andrew Yang on 4/14/21.
//

#ifndef RAYTRACING_2DHITTABLES_HPP
#define RAYTRACING_2DHITTABLES_HPP

#include <utility>

#include "hittable.hpp"
#include "vec3.hpp"

class plane : public hittable {
public:
    plane(point3 c, vec3 n, shared_ptr<material> m)
            : cen{c}, normal{n}, mat_ptr(std::move(m)) {};

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override {
        bool hit = false;
        float t;
        float den = (float)dot(normal, r.direction());
        if(std::abs(den) > .0001f) {
            t = (float)dot(cen - r.origin(), normal) / den;
            if(t >= .0001f) hit = true;
        }
        rec.t = t;
        rec.p = r.at(rec.t);
        rec.normal = normal;
        get_plane_uv(rec.p, rec.u, rec.v);
        rec.mat_ptr = mat_ptr;

        return hit;
    }

    bool bounding_box(float time0, float time1, aabb& output_box) const override {
        return false;
    }

private:
    point3 cen;
    shared_ptr<material> mat_ptr;

    static void get_plane_uv(const point3& p, float& u, float& v) {
        u = p.x() - floor(p.x());
        v = p.z() - floor(p.z());
    }

protected:
    vec3 normal;
};

class disk : public plane {
public:
    disk(point3 c, vec3 n, float r, shared_ptr<material> m)
            : plane(c, n, std::move(m)), radius{r}
    {}

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override {
        bool hit = false;
        if(plane::hit(r, t_min, t_max, rec)) {
            point3 p = r.direction() * rec.t + r.origin();
            vec3 v = p - cen;
            auto d2 = (float)dot(v, v);
            hit = d2 <= radius * radius;
        }

        return hit;
    }
    bool bounding_box(float time0, float time1, aabb& output_box) const override {
        if(plane::normal.x() == 1)
            output_box = aabb(cen - vec3(.01f, radius, radius), cen + vec3(.01f, radius, radius));
        else if(plane::normal.y() == 1)
            output_box = aabb(cen - vec3(radius, .01f, radius), cen + vec3(radius, .01f, radius));
        else
            output_box = aabb(cen - vec3(radius, radius, .01f), cen - vec3(radius, radius, .01f));
        return true;
    }

private:
    point3 cen;
    float radius;
};

#endif //RAYTRACING_2DHITTABLES_HPP
