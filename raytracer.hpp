//
// Created by Andrew Yang on 3/27/21.
//

#ifndef RAYTRACING_RAYTRACER_HPP
#define RAYTRACING_RAYTRACER_HPP

#include "rtweekend.hpp"
#include "camera.hpp"

#include "color.hpp"
#include "hittable/hittable_list.hpp"
#include "modifiers/material.hpp"

//color ray_color(const ray& r, const color& background, const hittable_list* world,
//                shared_ptr<hittable_list>& lights, int depth) {
//    hit_record rec;
//
//    // If we've exceeded the ray bounce limit, no more light is gathered.
//    if(depth <= 0)
//        return color(0, 0, 0);
//
//    // If the ray hits nothing, return the background color.
//    if(!world->hit(r, 0.001f, f_infinity, rec))
//        return background;
//    scatter_record srec;
//    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
//    if(!rec.mat_ptr->scatter(r, rec, srec))
//        return emitted;
//
//    if (srec.is_specular) {
//        return srec.attenuation
//               * ray_color(srec.specular_ray, background, world, lights, depth-1);
//    }
//
//    auto light_ptr = make_shared<hittable_pdf>(lights, rec.p);
//    mixture_pdf p(light_ptr, srec.pdf_ptr);
//
//    ray scattered = ray(rec.p, unit_vector(p.generate()), r.time());
//    auto pdf_val = p.value(scattered.direction());
//
//    return emitted + srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scattered)
//        * ray_color(scattered, background, world, lights, depth-1) / pdf_val;
//}

color ray_color2(const ray& r, const color& background, const hittable_list* world, int depth) {
    ray r_in = r;
    color rcolor = color(1,1,1);
    while(true) {
        hit_record rec;
        if(depth <= 0)
            return color(0,0,0);
        if(!world->hit(r_in, 0.001f, f_infinity, rec))
            return background * rcolor;
        scatter_record srec;
        color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
        if(!rec.mat_ptr->scatter(r_in, rec, srec))
            return emitted * rcolor;

        if(srec.is_specular) {
            r_in = srec.specular_ray;
            rcolor = srec.attenuation * rcolor;
            --depth;
            continue;
        }

        r_in = srec.specular_ray;
        --depth;
        rcolor = emitted + srec.attenuation * rec.mat_ptr->scattering_pdf(r_in, rec, srec.specular_ray) * rcolor / srec.pdf;
    }
}

#endif