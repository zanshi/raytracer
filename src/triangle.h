//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include <utility>

#include "common.h"
#include "colordbl.h"
#include "direction.h"
#include "vertex.h"
#include "shape.h"
#include "ray.h"
#include "trianglemesh.h"
#include <Eigen/Dense>

namespace rays {
    struct Triangle final : public Shape {

       explicit Triangle(const std::shared_ptr<TriangleMesh>& m, std::array<unsigned int, 3> inds, Direction n, ColorDbl c)
                    : mesh(m), indices(inds), normal(std::move(n)), Shape(c) {}

        bool rayIntersection(Ray &ray) const override {
            // Möller-Trumbore
            const Vertex& v0 = mesh->V->operator[](indices[0]);
            const Vertex& v1 = mesh->V->operator[](indices[1]);
            const Vertex& v2 = mesh->V->operator[](indices[2]);

            const Vertex E1 = v1 - v0;
            const Vertex E2 = v2 - v0;
            const Vertex T = ray.o - v0;


            return false;
        }

        std::shared_ptr<TriangleMesh> mesh;
        std::array<unsigned int, 3> indices;
        Direction normal;

    };
}
#endif //RAYTRACER_TRIANGLE_H
