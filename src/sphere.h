//
// Created by Niclas Olmenius on 2017-09-08.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "common.h"
#include "shape.h"
#include "vertex.h"
#include "ray.h"

#include <Eigen/Dense>

namespace rays {
    struct Sphere final : public Shape {
        explicit Sphere(const ColorDbl &c, Vertex center, float radius = 0.f)
                : Shape(c), center(center), r(radius) { }

        bool rayIntersection(Ray &ray) const override {
            const Vertex d = ray.e - ray.o;
            const float a = d.dot(d);
            const float b = 2.0f * (d.dot(ray.o - center));
            const float c = (ray.o - center).dot(ray.o - center) - (r * r);

            const float discrim = std::powf((b / 2.0f), 2) - a * c;

            if (discrim < 0) { return false; } // No real solutions
            const float rootDiscrim = std::sqrtf(discrim);

            const auto d1 = -( b / 2.0f ) + rootDiscrim;
            const auto d2 = -( b / 2.0f ) - rootDiscrim;

            ray.e = ray.o + std::min(d1, d2) * d;

            return true;
        }

        const Vertex center;
        const float r{0.f};

    };
}

#endif //RAYTRACER_SPHERE_H
