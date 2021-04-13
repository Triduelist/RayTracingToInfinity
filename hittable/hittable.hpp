//
// Created by Andrew Yang on 3/22/21.
//

#ifndef RAYTRACING_HITTABLE_HPP
#define RAYTRACING_HITTABLE_HPP

#include <utility>

#include "ray.hpp"
#include "rtweekend.hpp"
#include "aabb.hpp"

class material;

struct hit_record {
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    float t;
    float u;
    float v;
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal:-outward_normal;
    }
};

class hittable {
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(float time0, float time1, aabb& output_box) const = 0;
};

class translate : public hittable {
public:
    translate(shared_ptr<hittable> p, const vec3& displacement)
            : ptr(std::move(p)), offset(displacement)
    {}

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

    bool bounding_box(float time0, float time1, aabb& output_box) const override;

public:
    shared_ptr<hittable> ptr;
    vec3 offset;
};

bool translate::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    ray moved_r(r.origin() - offset, r.direction(), r.time());
    if (!ptr->hit(moved_r, t_min, t_max, rec))
        return false;

    rec.p += offset;
    rec.set_face_normal(moved_r, rec.normal);

    return true;
}

bool translate::bounding_box(float time0, float time1, aabb& output_box) const {
    if (!ptr->bounding_box(time0, time1, output_box))
        return false;

    output_box = aabb(
            output_box.min() + offset,
            output_box.max() + offset);

    return true;
}

class flip_face : public hittable {
public:
    flip_face(shared_ptr<hittable> p) : ptr(std::move(p)) {}

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override {
        if(!ptr->hit(r, t_min, t_max, rec))
            return false;

        rec.front_face = !rec.front_face;
        return true;
    }

    bool bounding_box(float time0, float time1, aabb& output_box) const override {
        return ptr->bounding_box(time0, time1, output_box);
    }

public:
    shared_ptr<hittable> ptr;
};

#endif //RAYTRACING_HITTABLE_HPP
