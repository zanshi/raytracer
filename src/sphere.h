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
                : Shape(c), center(std::move(center)), r(radius) { }

        bool rayIntersection(Ray &ray) const override {
            const Eigen::Vector4f nD {ray.d[0], ray.d[1], ray.d[2], 1.0};
            const float a = ray.d.dot(ray.d);
            const float b = 2.0f * (nD.dot(ray.o - center));
            const float c = (ray.o - center).dot(ray.o - center) - (r * r);

            const float d1 = -( b / 2.0f ) + std::sqrtf(std::powf((b / 2.0f), 2) - a * c);
            const float d2 = -( b / 2.0f ) - std::sqrtf(std::powf((b / 2.0f), 2) - a * c);

            // return what?

            return false;
        }

        const Vertex center;
        const float r{0.f};

    };
}

#endif //RAYTRACER_SPHERE_H
