//
// Created by Niclas Olmenius on 2017-09-08.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "common.h"
#include "shape.h"
#include "vector.h"

namespace rays {
    class Sphere final : public Shape {
    public:
        Sphere(const glm::vec3 &center, const float r) : center(center), r(r) {}

        ~Sphere() final = default;

        bool intersect(const Ray &ray, IntersectionInfo *isect, float *tHit) const override;

        float area() const override;

        glm::vec3 getRandomPoint(glm::vec2 r2) const override;

    private:
        const glm::vec3 center;
        const float r{1.f};

    };
}

#endif //RAYTRACER_SPHERE_H
