//
// Created by Andrew Yang on 4/14/21.
//

#ifndef RAYTRACING_TRIANGLES_HPP
#define RAYTRACING_TRIANGLES_HPP

#include <utility>

#include "hittable.hpp"
#include "vec3.hpp"

class triangle : public hittable {
public:
    triangle() = default;
    triangle(point3 p1, point3 p2, point3 p3, shared_ptr<material> m)
        : v1(p1), v2(p2), v3(p3), mat_ptr(std::move(m))
    {}

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
    bool bounding_box(float t0, float t1, aabb& output_box) const override;

public:
    point3 v1;
    point3 v2;
    point3 v3;
    shared_ptr<material> mat_ptr;
};

bool triangle::bounding_box(float t0, float t1, aabb &output_box) const {
    point3 center = (v1 + v2 + v3) / 3;
    // Find furthest vertex
    float radius = std::fabs((center - v1).length());
    float temp = std::fabs((center - v2).length());

    if(temp > radius) radius = temp;

    temp = std::fabs((center - v3).length());

    if(temp > radius) radius = temp;

    // Create bounding box
    output_box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
    return true;
}

bool triangle::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    // Implements the Moller-Trumbore intersection algorithm
    float tolerance = .0001f;

    vec3 head = r.direction();
    vec3 tail = r.origin();

    vec3 e1 = v2 - v1;
    vec3 e2 = v3 - v1;

    vec3 h = cross(head, e2);
    float a = dot(e1, h);

    // Check if ray is parallel to triangle
    if(a > -tolerance && a < tolerance)
        return false;

    float f = 1.0f/a;
    vec3 s = tail - v1;
    float u = f * dot(s, h);

    if(u < .0f || u > 1.0f)
        return false;

    vec3 q = cross(s, e1);
    float v = f * dot(head, q);

    if(v < 0.0f || u + v > 1.0f)
        return false;

    float t = f * dot(e2, q);

    if(t_min < t && t < t_max) {
        // Ray intersection
        rec.t = t;
        rec.p = r.at(rec.t);
        vec3 outward_normal = unit_vector(cross(e1, e2));
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mat_ptr;
        return true;
    }

    // Path intersection but not a ray intersection
    return false;
}
#endif //RAYTRACING_TRIANGLES_HPP
