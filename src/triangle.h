//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "common.h"
#include "trianglemesh.h"
#include "shape.h"
#include "rng.h"

namespace rays {

    struct Triangle final : public Shape {

        Triangle(std::shared_ptr<TriangleMesh> mesh, const std::array<unsigned int, 3> &indices) : mesh(
                std::move(mesh)), indices(indices), normal(glm::normalize(
                glm::cross(getV1() - getV0(), getV2() - getV0()))) {}

        ~Triangle() final = default;

        bool intersect(const Ray &ray, IntersectionInfo *isect, float *tHit) const override;

        glm::vec3 getRandomPoint(RNG &rng) const override;

        float area() const override;

        const glm::vec3 &getV0() const;

        const glm::vec3 &getV1() const;

        const glm::vec3 &getV2() const;

        const std::shared_ptr<TriangleMesh> mesh;
        const std::array<unsigned int, 3> indices;
        const glm::vec3 normal;

    };
}
#endif //RAYTRACER_TRIANGLE_H
