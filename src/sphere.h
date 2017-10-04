//
// Created by Niclas Olmenius on 2017-09-08.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "common.h"
#include "shape.h"
#include "vertex.h"

namespace rays {
    struct Sphere final : public Shape {

        Sphere(const Vertex3f &center, const float r) : center(center), r(r) {}

        ~Sphere() final = default;

        bool intersect(Ray &ray, IntersectionInfo *isect, float *tHit) const override;

        float area() const override;

        const Vertex3f center;
        const float r{1.f};

    };
}

#endif //RAYTRACER_SPHERE_H
