//
// Created by Andrew Yang on 4/3/21.
//

#ifndef RAYTRACING_BOX_HPP
#define RAYTRACING_BOX_HPP

#include "rtweekend.hpp"

#include "2dhittables.hpp"
#include "hittable_list.hpp"

class box : public hittable {
public:
    box() = default;

    box(const point3 &p0, const point3 &p1, const shared_ptr<material>& ptr);

    box(const point3 &p0, const point3 &p1, std::vector<shared_ptr<material>> &ptrs);

    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override;

    bool bounding_box(float time0, float time1, aabb &output_box) const override {
        output_box = aabb(box_min, box_max);
        return true;
    }

public:
    point3 box_min;
    point3 box_max;
    hittable_list sides;
};

box::box(const point3& p0, const point3& p1, const shared_ptr<material>& ptr) {
    box_min = p0;
    box_max = p1;

    sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr)); // front
    sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr)); // back

    sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr)); // top
    sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr)); // bottom

    sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr)); // left
    sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr)); // right
}

box::box(const point3& p0, const point3& p1, std::vector<shared_ptr<material>>& ptrs) {
    box_min = p0;
    box_max = p1;

    sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptrs[0])); // front
    sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptrs[1], true)); // back

    sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptrs[2], true)); // top
    sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptrs[3])); // bottom

    sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptrs[4], true)); // left
    sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptrs[5])); // right
}

bool box::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    if(!sides.hit(r, t_min, t_max, rec)) return false;
    return true;
}

#endif //RAYTRACING_BOX_HPP
