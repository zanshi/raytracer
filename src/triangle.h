//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include <utility>

#include "common.h"
#include "direction.h"
#include "trianglemesh.h"
#include "shape.h"

namespace rays {

#define TEST_CULL true

    struct Triangle final : public Shape {

        explicit Triangle(std::shared_ptr<TriangleMesh> m, std::array<unsigned int, 3> inds, Direction n,
                          ColorDbl c)
                : mesh(std::move(m)), indices(inds), normal(std::move(n)), Shape(c) {}

        ~Triangle() final = default;

        bool rayIntersection(Ray &ray) const override;

        std::shared_ptr<TriangleMesh> mesh;
        std::array<unsigned int, 3> indices;
        Direction normal;

    };
}
#endif //RAYTRACER_TRIANGLE_H
