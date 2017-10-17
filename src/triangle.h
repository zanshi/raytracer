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
#include "rng.h"

namespace rays {

#define TEST_CULL true

    struct Triangle final : public Shape {

        Triangle(const std::shared_ptr<TriangleMesh> &mesh, const std::array<unsigned int, 3> &indices,
                 const Vector3f &normal) : mesh(mesh), indices(indices), normal(normal) {}

        ~Triangle() final = default;

        bool intersect(Ray &ray, IntersectionInfo *isect, float *tHit) const override;

        Vertex3f getRandomPoint(RNG &rng) const;

        float area() const override;

        const Vertex3f& getV0() const;
        const Vertex3f& getV1() const;
        const Vertex3f& getV2() const;

        std::shared_ptr<TriangleMesh> mesh;
        std::array<unsigned int, 3> indices;
        Vector3f normal;

    };
}
#endif //RAYTRACER_TRIANGLE_H
