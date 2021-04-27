//
// Created by Andrew Yang on 4/9/21.
//

#ifndef RAYTRACING_SCENES_HPP
#define RAYTRACING_SCENES_HPP


#include "rtweekend.hpp"
#include "camera.hpp"

#include "color.hpp"
#include "hittable/hittable_list.hpp"
#include "hittable/sphere.hpp"
#include "hittable/rectangles.hpp"
#include "modifiers/material.hpp"
#include "hittable/moving_sphere.hpp"
#include "hittable/box.hpp"
#include "modifiers/rotate.hpp"
#include "modifiers/constant_medium.hpp"
#include "hittable/bvh.hpp"
#include "hittable/cylinder.hpp"
#include "hittable/cone.hpp"
#include "hittable/2dhittables.hpp"
#include "hittable/mesh.hpp"

hittable_list random_scene() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(color(.2f, .3f, .1f), color(.9f, .9f, .9f));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

    for(int a = -11; a < 11; ++a) {
        for(int b = -11; b < 11; ++b) {
            auto choose_mat = random_float();
            point3 center((float)a + .9f*random_float(), .2, (float)b + .9f*random_float());

            if((center - point3(4, .2, 0)).length() > .9) {
                shared_ptr<material> sphere_material;

                if(choose_mat < .8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
//                    auto center2 = center + vec3(0, random_float(0, .5), 0);
//                    world.add(make_shared<moving_sphere>(center, center2, .0f, 1.0f, .2f, sphere_material));
                    world.add(make_shared<sphere>(center, .2f, sphere_material));
                }
                else if(choose_mat < .95) {
                    // metal
                    auto albedo = color::random(.5, 1);
                    auto fuzz = random_float(0, .5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, .2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, .2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(.4, .2, .1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(.7, .6, .5), 0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

hittable_list cornell_box() {
    hittable_list objects;

    auto red = make_shared<lambertian>(color(.65f, .05f, .05f));
    auto white = make_shared<lambertian>(color(.73f, .73f, .73f));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(123, 423, 147, 412, 554, light)));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    //shared_ptr<material> aluminum = make_shared<metal>(color(.8f, .85f, .88f), .0f);
    shared_ptr<hittable> box1 = make_shared<box>(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    objects.add(box1);

    shared_ptr<hittable> box2 = make_shared<box>(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));
    objects.add(box2);

    return objects;
}

hittable_list cornell_glass() {
    hittable_list objects;

    auto red = make_shared<lambertian>(color(.65f, .05f, .05f));
    auto white = make_shared<lambertian>(color(.73f, .73f, .73f));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    auto glass = make_shared<dielectric>(1.5);
    shared_ptr<material> aluminum = make_shared<metal>(color(.8f, .85f, .88f), .5f);

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(123, 423, 147, 412, 554, light)));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> cyl = make_shared<cylinder>(point3(0,0,0), 110.0f, 400.0f, glass);
    cyl = make_shared<rotate_x>(cyl, 90);
    cyl = make_shared<translate>(cyl, vec3(365, 200, 395));
    objects.add(cyl);

    shared_ptr<hittable> cne = make_shared<cone>(point3(0,0,0), 54.f, 345.6f, 172.8f, glass);
    cne = make_shared<rotate_y>(cne, 110);
    cne = make_shared<rotate_z>(cne, -17.5);
    cne = make_shared<translate>(cne, vec3(260, 52, 100));
    objects.add(cne);

    return objects;
}

hittable_list cornell_smoke() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7,7,7));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(123, 423, 147, 412, 554, light)));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));

    shared_ptr<hittable> box2 = make_shared<box>(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));

    objects.add(make_shared<constant_medium>(box1, 0.01, color(0,0,0)));
    objects.add(make_shared<constant_medium>(box2, 0.01, color(1,1,1)));

    return objects;
}

hittable_list moving_spheres() {
    hittable_list spheres;
    auto col1 = make_shared<diffuse_light>(color(.3f, .93f, .91f));
    auto col2 = make_shared<diffuse_light>(color(.45f, .93f, .08f));
    auto col3 = make_shared<diffuse_light>(color(.99f, .90f, .0f));
    auto col4 = make_shared<diffuse_light>(color(.0f, .12f, .99f));
    auto col5 = make_shared<diffuse_light>(color(.94f, .0f, .99f));
    auto light = make_shared<diffuse_light>(color(7,7,7));

    const int spheres_per_side = 5;
    for(int i=0; i < spheres_per_side; ++i) {
        for(int j=0; j < spheres_per_side; ++j) {
            for(int k=0; k < spheres_per_side; ++k) {
                auto col = make_shared<diffuse_light>(color(0,0,0));
                int rand = random_int(0, 4);
                if(rand == 0) col = col1;
                else if(rand == 1) col = col2;
                else if(rand == 2) col = col3;
                else if(rand == 3) col = col4;
                else col = col5;

                float w = 150.0f;
                vec3 move = random_in_unit_sphere() * 75.0f;
                float x = (-500.0f + (float)i*w);
                float y = (-500.0f + (float)j*w);
                float z = (-500.0f + (float)k*w);

                point3 center = point3(x,y,z) + random_in_unit_sphere() * 100.0f;
                spheres.add(make_shared<moving_sphere>(center, center + move, 0, 2, 12.f, col));
            }
        }
    }

    hittable_list objects;

    objects.add(make_shared<bvh_node>(spheres, 0, 1));
    objects.add(make_shared<xz_rect>(178, 578, 100, 500, 0, light));
    objects.add(make_shared<plane>(point3(0,250,0), vec3(0,1,0), light));

    return objects;
}

hittable_list final_scene() {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(.48f, .83f, .53f));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0f;
            auto x0 = -1000.0f + (float)i*w;
            auto z0 = -1000.0f + (float)j*w;
            auto y0 = 0.0f;
            auto x1 = x0 + w;
            auto y1 = random_float(1,101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<box>(point3(x0,y0,z0), point3(x1,y1,z1), ground));
        }
    }

    hittable_list objects;

    objects.add(make_shared<bvh_node>(boxes1, 0, 1));

    auto light = make_shared<diffuse_light>(color(7,7,7));
    objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

    auto center1 = point3(400, 400, 400);
    auto center2 = center1 + vec3(30, 0, 0);
    auto moving_sphere_material = make_shared<lambertian>(color(.7f, .3f, .1f));
    objects.add(make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    objects.add(make_shared<sphere>(point3(0, 150, 145), 50, make_shared<metal>(color(.8f, .8f, .9f), 1.0f)));

    auto boundary = make_shared<sphere>(point3(360, 150, 145), 70, make_shared<dielectric>(1.5));
    objects.add(boundary);
    objects.add(make_shared<constant_medium>(boundary, .2f, color(.2f, .4f, .9f)));
    boundary = make_shared<sphere>(point3(0,0,0), 5000, make_shared<dielectric>(1.5f));
    objects.add(make_shared<constant_medium>(boundary, .0001f, color(1,1,1)));

    auto emat = make_shared<lambertian>(make_shared<image_texture>("resources/earthmap.jpeg"));
    objects.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));
    auto pertext = make_shared<noise_texture>(.1f);
    objects.add(make_shared<sphere>(point3(220, 280, 300), 80, make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73f, .73f, .73f));
    int ns = 1000;
    for(int j=0; j < ns; ++j) {
        boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));
    }

    objects.add(make_shared<translate>(
            make_shared<rotate_y>(make_shared<bvh_node>(boxes2, 0.0f, 1.0f), 15),
            vec3(-100, 270, 395)
                )
    );

    return objects;
}

hittable_list single_cylinder() {
    hittable_list objects;

    auto material1 = make_shared<dielectric>(1.5);
    shared_ptr<hittable> cyl = make_shared<cylinder>(point3(0, 1, 0), 1.0f, 2.0f, material1);
    rotate(cyl, 40, -18, 0);
    cyl = make_shared<rotate_y>(cyl, 40);

    objects.add(cyl);
    return objects;
}

hittable_list single_cone() {
    hittable_list objects;

    objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(color(.2, .7, .2))));

    auto material1 = make_shared<lambertian>(color(.7f, .7f, .7f));
    shared_ptr<hittable> cne = make_shared<cone>(point3(0, .5f, 0), .4f, 2.f, 1.f, material1);
    cne = make_shared<rotate_x>(cne, -22);

    objects.add(cne);
    return objects;
}

hittable_list mapped_box() {
    hittable_list objects;

    objects.add(make_shared<sphere>(point3(3, -1, 0), 1.f, make_shared<metal>(color(.7f,.7f,.7f), 0)));

    std::vector<shared_ptr<material>> skybox = {
            make_shared<diffuse_light>(make_shared<image_texture>("resources/stor/posz.jpg")),
            make_shared<diffuse_light>(make_shared<image_texture>("resources/stor/negz.jpg")),
            make_shared<diffuse_light>(make_shared<image_texture>("resources/stor/posy.jpg")),
            make_shared<diffuse_light>(make_shared<image_texture>("resources/stor/negy.jpg")),
            make_shared<diffuse_light>(make_shared<image_texture>("resources/stor/posx.jpg")),
            make_shared<diffuse_light>(make_shared<image_texture>("resources/stor/negx.jpg"))
    };


    auto emat = make_shared<lambertian>(make_shared<image_texture>("resources/earthmap.jpeg"));
    shared_ptr<hittable> box1 = make_shared<box>(point3(-10,-10,-10), point3(10,10,10), skybox);
    objects.add(box1);

    return objects;
}

hittable_list gold_coin() {
    hittable_list objects;

    auto aluminum = make_shared<metal>(color(.5f,.5f,.5f), .0f);
    auto light = make_shared<diffuse_light>(color(12,12,12));

    auto gold = make_shared<lambertian>(color(0.843137255f,0.717647059f,0.250980392f));
    auto object = make_shared<mesh>("resources/coin_reduced.obj", gold, point3(0,.2,0), .1f);
    objects.add(object);

    objects.add(make_shared<flip_face>(make_shared<yz_rect>(0, 1.5f, -5, 10, 5, light)));

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    return objects;
}

hittable_list mesh_test() {
    hittable_list objects;

    auto aluminum = make_shared<metal>(color(.5f,.5f,.5f), .0f);
    auto red = make_shared<lambertian>(color(.7f,.5f,.5f));

    auto light = make_shared<diffuse_light>(color(12,12,12));

    auto object = make_shared<mesh>("resources/teapot.obj", red, point3(0,.2,0), .7f);
    objects.add(object);

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    return objects;
}

#endif //RAYTRACING_SCENES_HPP
