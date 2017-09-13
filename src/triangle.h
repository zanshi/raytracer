//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include <utility>

#include "common.h"
#include "colordbl.h"
#include "direction.h"
#include "vector.h"
#include "vertex.h"
#include "ray.h"
#include "shape.h"
#include "trianglemesh.h"

namespace rays {

#define TEST_CULL true

    struct Triangle final : public Shape {

        using limits = std::numeric_limits<float>;

        explicit Triangle(const std::shared_ptr<TriangleMesh> &m, std::array<unsigned int, 3> inds, Direction n,
                          ColorDbl c)
                : mesh(m), indices(inds), normal(std::move(n)), Shape(c) {}

        bool rayIntersection(Ray &ray) const override {
            // Möller-Trumbore
            // Based on the canonical implementation
            const Vertex &v0 = mesh->V->operator[](indices[0]);
            const Vertex &v1 = mesh->V->operator[](indices[1]);
            const Vertex &v2 = mesh->V->operator[](indices[2]);

            const Vertex e1 = v1 - v0;
            const Vertex e2 = v2 - v0;
            const Vertex dir = ray.e - ray.o;
            const Vertex P = dir.cross(e2);

            // Use determinant to bail out early in case the triangle is facing away
            double det = e1.dot(P);

//#ifdef TEST_CULL
            if (det < std::numeric_limits<float>::epsilon()) {
                return false;
            }
            // Test bounds for u
            const Vertex T = ray.o - v0;
            double u = T.dot(P);
            if (u < 0.0 || u > det) {
                return false;
            }

            // Test bounds for v
            const Vertex Q = T.cross(e1);
            double v = dir.dot(Q);
            if ( v < 0.0 || u + v > det) {
                return false;
            }

            // We have a match!
            // Calculate t
            double t = Q.dot(e2);
            double invDet = 1.0 / det;
            t *= invDet;
            u *= invDet;
            v *= invDet;

            // Update ray intersection point
            ray.e = ray.o + t * dir;

//#endif
            return true;
        }

        std::shared_ptr<TriangleMesh> mesh;
        std::array<unsigned int, 3> indices;
        Direction normal;

    };
}
#endif //RAYTRACER_TRIANGLE_H
