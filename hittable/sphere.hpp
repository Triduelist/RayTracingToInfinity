//
// Created by Andrew Yang on 3/22/21.
//

#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include <utility>

#include "hittable.hpp"
#include "aabb.hpp"
#include "vec3.hpp"
#include "onb.hpp"

class sphere : public hittable {
public:
    sphere(point3 cen, float r, shared_ptr<material> m)
        : center(cen), radius(r), mat_ptr(std::move(m)) {};

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

    bool bounding_box(float time0, float time1, aabb& output_box) const override;

private:
    point3 center;
    float radius;
    shared_ptr<material> mat_ptr;

    static void get_sphere_uv(const point3& p, float &u, float &v) {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        float phi = atan2(p.z(), p.x());
        float theta = asin(p.y());
        u = 1 - (phi + fpi) / (2 * fpi);
        v = (theta + fpi / 2) / fpi;
    }

    static vec3 random_to_sphere(float radius, float distance_squared) {
        auto r1 = random_float();
        auto r2 = random_float();
        auto z = 1 + r2*(sqrtf(1.f-radius*radius/distance_squared) - 1);

        auto phi = 2*fpi*r1;
        auto x = cosf(phi)*sqrtf(1.f-z*z);
        auto y = sinf(phi)*sqrtf(1.f-z*z);

        return vec3(x, y, z);
    }
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if(discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    //find the nearest root that lies in the acceptable range
    auto root = (-half_b - sqrtd) / a;
    if(root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if(root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;

    return true;
};

bool sphere::bounding_box(float time0, float time1, aabb &output_box) const {
    output_box = aabb(
            center - vec3(radius, radius, radius),
            center + vec3(radius, radius, radius));
    return true;
}

#endif //RAYTRACING_SPHERE_H
