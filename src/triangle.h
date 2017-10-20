//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include <utility>
#include <xmmintrin.h>

#include "common.h"
#include "direction.h"
#include "trianglemesh.h"
#include "shape.h"
#include "rng.h"

namespace rays {

    struct Triangle final : public Shape {

        Triangle(std::shared_ptr<TriangleMesh> mesh, const std::array<unsigned int, 3> &indices) : mesh(
                std::move(mesh)), indices(indices), normal(glm::normalize(
                glm::cross(getV2() - getV0(), getV1() - getV0()))) {}

        ~Triangle() final = default;

        bool intersect(Ray &ray, IntersectionInfo *isect, float *tHit) const override;

        Vertex3f getRandomPoint(RNG &rng) const override;

        float area() const override;

        const Vertex3f &getV0() const;

        const Vertex3f &getV1() const;

        const Vertex3f &getV2() const;

        std::shared_ptr<TriangleMesh> mesh;
        std::array<unsigned int, 3> indices;
        const Vector3f normal;

    };
}
#endif //RAYTRACER_TRIANGLE_H
