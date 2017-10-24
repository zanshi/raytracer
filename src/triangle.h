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

        explicit Triangle(std::array<glm::vec3, 3> verts) : V(verts),
                                                   normal(glm::normalize(glm::cross(V[1] - V[0], V[2] - V[0]))) {}

        ~Triangle() final = default;

        bool intersect(const Ray &ray, IntersectionInfo *isect, float *tHit) const override;

        glm::vec3 getRandomPoint(RNG &rng) const override;

        float area() const override;

        const std::array<glm::vec3, 3> V;
        const glm::vec3 normal;

    };
}
#endif //RAYTRACER_TRIANGLE_H
