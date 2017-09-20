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
        explicit Sphere(const ColorDbl &c, Vertex center, float radius = 0.f)
                : Shape(c), center(center), r(radius) {}

        ~Sphere() final = default;

        bool rayIntersection(Ray &ray) const override;

        float area() const override;

        const Vertex center;
        const float r{1.f};

    };
}

#endif //RAYTRACER_SPHERE_H
