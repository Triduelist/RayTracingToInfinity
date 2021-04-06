//
// Created by Andrew Yang on 4/3/21.
//

#ifndef RAYTRACING_ROTATE_HPP
#define RAYTRACING_ROTATE_HPP

#include <utility>

#include "hittable/hittable.hpp"

class rotate_y : public hittable {
public:
    rotate_y(shared_ptr<hittable> p, float angle);

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

    bool bounding_box(float time0, float time1, aabb& output_box) const override {
        output_box = bbox;
        return hasbox;
    }

public:
    shared_ptr<hittable> ptr;
    float sin_theta;
    float cos_theta;
    bool hasbox;
    aabb bbox;
};

rotate_y::rotate_y(shared_ptr<hittable> p, float angle) : ptr(std::move(p)) {
    auto radians = degrees_to_radians(angle);
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = ptr->bounding_box(0, 1, bbox);

    point3 min( f_infinity,  f_infinity,  f_infinity);
    point3 max(-f_infinity, -f_infinity, -f_infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = (float)i*bbox.max().x() + (1-(float)i)*bbox.min().x();
                auto y = (float)j*bbox.max().y() + (1-(float)j)*bbox.min().y();
                auto z = (float)k*bbox.max().z() + (1-(float)k)*bbox.min().z();

                auto newx =  cos_theta*x + sin_theta*z;
                auto newz = -sin_theta*x + cos_theta*z;

                vec3 tester(newx, y, newz);

                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox = aabb(min, max);
}

bool rotate_y::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    auto origin = r.origin();
    auto direction = r.direction();

    origin[0] = cos_theta*r.origin()[0] - sin_theta*r.origin()[2];
    origin[2] = sin_theta*r.origin()[0] + cos_theta*r.origin()[2];

    direction[0] = cos_theta*r.direction()[0] - sin_theta*r.direction()[2];
    direction[2] = sin_theta*r.direction()[0] + cos_theta*r.direction()[2];

    ray rotated_r(origin, direction, r.time());

    if (!ptr->hit(rotated_r, t_min, t_max, rec))
        return false;

    auto p = rec.p;
    auto normal = rec.normal;

    p[0] =  cos_theta*rec.p[0] + sin_theta*rec.p[2];
    p[2] = -sin_theta*rec.p[0] + cos_theta*rec.p[2];

    normal[0] =  cos_theta*rec.normal[0] + sin_theta*rec.normal[2];
    normal[2] = -sin_theta*rec.normal[0] + cos_theta*rec.normal[2];

    rec.p = p;
    rec.set_face_normal(rotated_r, normal);

    return true;
}

class rotate_x : public hittable {
public:
    rotate_x(shared_ptr<hittable> p, float angle);

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

    bool bounding_box(float time0, float time1, aabb& output_box) const override {
        output_box = bbox;
        return hasbox;
    }

public:
    shared_ptr<hittable> ptr;
    float sin_theta;
    float cos_theta;
    bool hasbox;
    aabb bbox;
};

rotate_x::rotate_x(shared_ptr<hittable> p, float angle) : ptr(std::move(p)) {
    auto radians = degrees_to_radians(angle);
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = ptr->bounding_box(0, 1, bbox);

    point3 min( f_infinity,  f_infinity,  f_infinity);
    point3 max(-f_infinity, -f_infinity, -f_infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = (float)i*bbox.max().x() + (1-(float)i)*bbox.min().x();
                auto y = (float)j*bbox.max().y() + (1-(float)j)*bbox.min().y();
                auto z = (float)k*bbox.max().z() + (1-(float)k)*bbox.min().z();

                auto newy =  cos_theta*y + sin_theta*z;
                auto newz = -sin_theta*y + cos_theta*z;

                vec3 tester(x, newy, newz);

                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox = aabb(min, max);
}

bool rotate_x::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    auto origin = r.origin();
    auto direction = r.direction();

    origin[1] = cos_theta*r.origin()[1] - sin_theta*r.origin()[2];
    origin[2] = sin_theta*r.origin()[1] + cos_theta*r.origin()[2];

    direction[1] = cos_theta*r.direction()[1] - sin_theta*r.direction()[2];
    direction[2] = sin_theta*r.direction()[1] + cos_theta*r.direction()[2];

    ray rotated_r(origin, direction, r.time());

    if (!ptr->hit(rotated_r, t_min, t_max, rec))
        return false;

    auto p = rec.p;
    auto normal = rec.normal;

    p[1] =  cos_theta*rec.p[1] + sin_theta*rec.p[2];
    p[2] = -sin_theta*rec.p[1] + cos_theta*rec.p[2];

    normal[1] =  cos_theta*rec.normal[1] + sin_theta*rec.normal[2];
    normal[2] = -sin_theta*rec.normal[1] + cos_theta*rec.normal[2];

    rec.p = p;
    rec.set_face_normal(rotated_r, normal);

    return true;
}

class rotate_z : public hittable {
public:
    rotate_z(shared_ptr<hittable> p, float angle);

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

    bool bounding_box(float time0, float time1, aabb& output_box) const override {
        output_box = bbox;
        return hasbox;
    }

public:
    shared_ptr<hittable> ptr;
    float sin_theta;
    float cos_theta;
    bool hasbox;
    aabb bbox;
};

rotate_z::rotate_z(shared_ptr<hittable> p, float angle) : ptr(std::move(p)) {
    auto radians = degrees_to_radians(angle);
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = ptr->bounding_box(0, 1, bbox);

    point3 min( f_infinity,  f_infinity,  f_infinity);
    point3 max(-f_infinity, -f_infinity, -f_infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = (float)i*bbox.max().x() + (1-(float)i)*bbox.min().x();
                auto y = (float)j*bbox.max().y() + (1-(float)j)*bbox.min().y();
                auto z = (float)k*bbox.max().z() + (1-(float)k)*bbox.min().z();

                auto newy =  cos_theta*y + sin_theta*x;
                auto newx = -sin_theta*y + cos_theta*x;

                vec3 tester(newx, newy, z);

                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox = aabb(min, max);
}

bool rotate_z::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    auto origin = r.origin();
    auto direction = r.direction();

    origin[1] = cos_theta*r.origin()[1] - sin_theta*r.origin()[0];
    origin[0] = sin_theta*r.origin()[1] + cos_theta*r.origin()[0];

    direction[1] = cos_theta*r.direction()[1] - sin_theta*r.direction()[0];
    direction[0] = sin_theta*r.direction()[1] + cos_theta*r.direction()[0];

    ray rotated_r(origin, direction, r.time());

    if (!ptr->hit(rotated_r, t_min, t_max, rec))
        return false;

    auto p = rec.p;
    auto normal = rec.normal;

    p[1] =  cos_theta*rec.p[1] + sin_theta*rec.p[0];
    p[0] = -sin_theta*rec.p[1] + cos_theta*rec.p[0];

    normal[1] =  cos_theta*rec.normal[1] + sin_theta*rec.normal[0];
    normal[0] = -sin_theta*rec.normal[1] + cos_theta*rec.normal[0];

    rec.p = p;
    rec.set_face_normal(rotated_r, normal);

    return true;
}

void rotate(shared_ptr<hittable>& p, float x, float y, float z) {
    std::cout << x << " " << y << " " << z << std::endl;
    p = make_shared<rotate_y>(p, y);
    p = make_shared<rotate_x>(p, x);
    p = make_shared<rotate_z>(p, z);
}

#endif //RAYTRACING_ROTATE_HPP
