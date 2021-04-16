//
// Created by Andrew Yang on 3/30/21.
//

#ifndef RAYTRACING_RECTANGLES_HPP
#define RAYTRACING_RECTANGLES_HPP

#include <utility>

#include "hittable.hpp"
#include "vec3.hpp"

class xy_rect : public hittable {
public:
    xy_rect() = default;

    xy_rect(float _x0, float _x1, float _y0, float _y1, float _k, shared_ptr<material> mat, bool _rev=false)
        : x0{_x0}, x1{_x1}, y0{_y0}, y1{_y1}, k{_k}, rev{_rev}, mp{std::move(mat)} {};

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override {
        auto t = (k-r.origin().z()) / r.direction().z();
        if(t < t_min || t > t_max)
            return false;
        auto x = r.origin().x() + t*r.direction().x();
        auto y = r.origin().y() + t*r.direction().y();
        if(x < x0 || x > x1 || y < y0 || y > y1)
            return false;
        rec.u = rev ? ((x1-x)/(x1-x0)):((x-x0)/(x1-x0));
        rec.v = (y-y0)/(y1-y0);
        rec.t = t;
        auto outward_normal = vec3(0, 0, 1);
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mp;
        rec.p = r.at(t);
        return true;
    }

    bool bounding_box(float time0, float time1, aabb& output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the Z dimension a small amount
        output_box = aabb(point3(x0, y0, k-.0001f), point3(x1, y1, k+.0001f));
        return true;
    }

public:
    shared_ptr<material> mp;
    float x0{}, x1{}, y0{}, y1{}, k{};
    bool rev;
};

class xz_rect : public hittable {
public:
    xz_rect() = default;

    xz_rect(float _x0, float _x1, float _z0, float _z1, float _k,shared_ptr<material> mat, bool _rev=false)
            : x0{_x0}, x1{_x1}, z0{_z0}, z1{_z1}, k{_k}, mp{std::move(mat)} {};

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override {
        auto t = (k-r.origin().y()) / r.direction().y();
        if (t < t_min || t > t_max)
            return false;
        auto x = r.origin().x() + t*r.direction().x();
        auto z = r.origin().z() + t*r.direction().z();
        if (x < x0 || x > x1 || z < z0 || z > z1)
            return false;
        rec.u = (x-x0)/(x1-x0);
        rec.v = (z-z0)/(z1-z0);
        rec.t = t;
        auto outward_normal = vec3(0, 1, 0);
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mp;
        rec.p = r.at(t);
        return true;
    }

    bool bounding_box(float time0, float time1, aabb& output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the Z dimension a small amount
        output_box = aabb(point3(x0, z0, k-.0001f), point3(x1, z1, k+.0001f));
        return true;
    }

public:
    shared_ptr<material> mp;
    float x0{}, x1{}, z0{}, z1{}, k{};
    bool rev;
};

class yz_rect : public hittable {
public:
    yz_rect() = default;

    yz_rect(float _y0, float _y1, float _z0, float _z1, float _k, shared_ptr<material> mat, bool _rev=false)
            : y0{_y0}, y1{_y1}, z0{_z0}, z1{_z1}, k{_k}, rev{_rev}, mp{std::move(mat)} {};

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override {
        auto t = (k-r.origin().x()) / r.direction().x();
        if (t < t_min || t > t_max)
            return false;
        auto y = r.origin().y() + t*r.direction().y();
        auto z = r.origin().z() + t*r.direction().z();
        if (y < y0 || y > y1 || z < z0 || z > z1)
            return false;
        rec.u = rev ? ((z1-z)/(z1-z0)):((z-z0)/(z1-z0));
        rec.v = (y-y0)/(y1-y0);
        rec.t = t;
        auto outward_normal = vec3(1, 0, 0);
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mp;
        rec.p = r.at(t);
        return true;
    }

    bool bounding_box(float time0, float time1, aabb& output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the Z dimension a small amount
        output_box = aabb(point3(y0, z0, k-.0001f), point3(y1, z1, k+.0001f));
        return true;
    }

public:
    shared_ptr<material> mp;
    float y0{}, y1{}, z0{}, z1{}, k{};
    bool rev;
};

#endif //RAYTRACING_RECTANGLES_HPP
