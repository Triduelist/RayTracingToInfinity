//
// Created by Andrew Yang on 3/23/21.
//

#ifndef RAYTRACING_MATERIAL_HPP
#define RAYTRACING_MATERIAL_HPP

#include <utility>

#include "rtweekend.hpp"
#include "texture.hpp"
#include "onb.hpp"

struct hit_record;

struct scatter_record {
    ray specular_ray;
    bool is_specular{};
    color attenuation;
    float pdf{};
};

class material {
public:
    [[nodiscard]] virtual color emitted(const ray& r_in, const hit_record& rec, float u, float v, const point3& p) const {
        return color(0,0,0);
    }
    virtual bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const {
        return false;
    }
    [[nodiscard]] virtual float scattering_pdf(const ray& r_in, const hit_record& rec, const ray& scattered) const {
        return 0;
    }
};

class lambertian : public material {
public:
    explicit lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
    explicit lambertian(shared_ptr<texture> a) : albedo(std::move(a)) {}

    bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const override {
        onb uvw;
        uvw.build_from_w(rec.normal);
        auto direction = uvw.local(random_cosine_direction());
        srec.specular_ray = ray(rec.p, unit_vector(direction), r_in.time());
        srec.pdf = dot(uvw.w(), srec.specular_ray.direction()) / fpi;
        srec.is_specular = false;
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }
    [[nodiscard]] float scattering_pdf(const ray& r_in, const hit_record& rec, const ray& scattered) const override {
        auto cosine = dot(rec.normal, unit_vector(scattered.direction()));
        return cosine < 0 ? 0 : cosine/pi;
    }

public:
    shared_ptr<texture> albedo;
};

class metal : public material {
public:
    metal(const color& a, float f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        srec.specular_ray = ray(rec.p, reflected+fuzz*random_in_unit_sphere());
        srec.attenuation = albedo;
        srec.is_specular = true;
        return true;
    }

public:
    color albedo;
    float fuzz;
};

class dielectric : public material {
public:
    explicit dielectric(float index_of_refraction, const color& a=color(1.f,1.f,1.f), float f=0.f)
            : ir(index_of_refraction), albedo(a), fuzz(f < 1.f ? f : 1.f) {}

    bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const override {
        srec.attenuation = albedo;
        srec.is_specular = true;
        float refraction_ratio = rec.front_face ? (1.0/ir) : ir;

        vec3 unit_direction = unit_vector(r_in.direction());
        float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        float sin_theta = sqrtf(1.0f - cos_theta*cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;

        if(cannot_refract || reflectance(cos_theta, refraction_ratio) > random_float())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, refraction_ratio);

        srec.specular_ray = ray(rec.p,
                                direction + (fuzz ? (fuzz*random_in_unit_sphere()):vec3(0,0,0)),
                            r_in.time());
        return true;
    }

public:
    float ir; // Index of refraction
    float fuzz;
    color albedo;
private:
    static float reflectance(float cosine, float ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*powf((1 - cosine),5);
    }
};
class diffuse_light : public material  {
public:
    diffuse_light(shared_ptr<texture> a) : emit(std::move(a)) {}
    explicit diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

    bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const override {
        return false;
    }

    [[nodiscard]] color emitted(const ray& r_in, const hit_record& rec, float u, float v, const point3& p) const override {
        if(rec.front_face)
            return emit->value(u, v, p);
        else
            return color(0,0,0);
    }

public:
    shared_ptr<texture> emit;
};

class isotropic : public material {
public:
    explicit isotropic(color c) : albedo(make_shared<solid_color>(c)) {}
    explicit isotropic(shared_ptr<texture> a) : albedo(std::move(a)) {}

    bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const override {
        srec.specular_ray = ray(rec.p, random_in_unit_sphere(), r_in.time());
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

public:
    shared_ptr<texture> albedo;
};

#endif //RAYTRACING_MATERIAL_HPP
