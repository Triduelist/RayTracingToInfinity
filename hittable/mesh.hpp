//
// Created by Andrew Yang on 4/21/21.
//

#ifndef RAYTRACING_MESH_HPP
#define RAYTRACING_MESH_HPP

#include <cstdlib>
#include <vector>

#include <fstream>
#include <sstream>

#include "bvh.hpp"
#include "hittable.hpp"
#include "triangles.hpp"

class mesh : public hittable {
public:
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
                int v[3];
                std::string temp{};
                for(int i=0; i < 3; ++i) {
                    linestream >> v[i];
                    if(linestream.peek() != ' ' && !isdigit(linestream.peek()))
                        linestream >> temp;
                }
                char c;
                int i;
//                linestream >> v[0] >> c >> i >> c >> i >> v[1] >> c >> i >> c >> i >> v[2];
//                linestream >> v0 >> c >> c >> i >> v1 >> c >> c >> i >> v2;
//                linestream >> v[0] >> v[1] >> v[2];
                faces.emplace_back(vertices[v[0] - 1], vertices[v[1] - 1], vertices[v[2] - 1], m);
            }
            linestream.str(std::string());
            linestream.clear();
        }
        for(auto & face : faces) {
            face = triangle(face.v1 * scale + origin, face.v2 * scale + origin, face.v3 * scale + origin, m);
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
