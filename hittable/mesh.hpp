//
// Created by Andrew Yang on 4/15/21.
//

#ifndef RAYTRACING_MESH_HPP
#define RAYTRACING_MESH_HPP

#include <cstdlib>
#include <vector>

#include <fstream>
#include <sstream>

#include "triangles.hpp"
#include "hittable.hpp"

class mesh : public hittable {
public:
    mesh() = default;
    mesh(const std::string& path, const shared_ptr<material>& m, point3 origin, float scale) {
        mat_ptr = m;

        std::ifstream in_file(path);
        std::string line, op;
        std::stringstream linestream;
        float x, y, z;

        while(std::getline(in_file, line)) {
            linestream << line;
            linestream >> op;

            if(op == "v") {
                linestream >> x >> y >> z;
                vertices.emplace_back(x,y,z);
            }
            else if(op == "f") {
                int v0, v1, v2, i;
                char c;
//                linestream >> v0 >> c >> i >> c >> i >> v1 >> c >> i >> c >> i >> v2;
                linestream >> v0 >> v1 >> v2;
                faces.emplace_back(vertices[v0 - 1], vertices[v1 - 1], vertices[v2 - 1], m);
                faces.back().v1 = faces.back().v1 * scale + origin;
                faces.back().v2 = faces.back().v2 * scale + origin;
                faces.back().v3 = faces.back().v3 * scale + origin;
            }

            linestream.str(std::string());
            linestream.clear();
        }
    }

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

    bool bounding_box(float t0, float t1, aabb& output_box) const override {
        return false;
    }

public:
    shared_ptr<material> mat_ptr;
    std::vector<vec3> vertices;
    std::vector<triangle> faces;
};

bool mesh::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    vec3 normal(0,0,0);
    float thit = 10000000.0f;
    bool hit = false;

    for(const triangle& f : faces) {
        if(f.hit(r, t_min, t_max, rec)) {
            if(rec.t < thit) {
                thit = rec.t;
                normal = rec.normal;
            }
            hit = true;
        }
    }

    rec.t = thit;
    rec.normal = normal;
    rec.p = r.at(rec.t);

    return hit;
}

#endif //RAYTRACING_MESH_HPP
