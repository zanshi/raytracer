//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "common.h"
#include "shape.h"
#include "rng.h"
#include <glm/glm.hpp>

namespace rays {

    class Triangle final : public Shape {
    public:
        explicit Triangle(std::array<glm::vec3, 3> verts)
                : V(verts),
                  normal(glm::normalize(glm::cross(V[1] - V[0], V[2] - V[0]))),
                  T(calculateAffineTransformation()) {}

        ~Triangle() final = default;

        bool intersect(const Ray &ray, IntersectionInfo *isect, float *tHit) const override;

        glm::vec3 getRandomPoint(const glm::vec2 &r2) const override;

        float area() const override;

    private:

        glm::mat3 calculateAffineTransformation();

        const std::array<glm::vec3, 3> V;
        const glm::vec3 normal;
        const glm::mat3 T;

    };
}
#endif //RAYTRACER_TRIANGLE_H
