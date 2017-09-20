//
// Created by Niclas Olmenius on 2017-09-15.
//

#include "sphere.h"
#include "ray.h"

namespace rays {

    bool Sphere::rayIntersection(Ray &ray) const {
        const Vector3f d = ray.e - ray.o;
        const float a = d.dot(d);
        const float b = 2.0f * (d.dot(ray.o - center));
        const float c = (ray.o - center).dot(ray.o - center) - (r * r);

        const auto discrim = std::pow((b / 2.0f), 2) - a * c;

        if (discrim < 0) { return false; } // No real solutions
        const auto rootDiscrim = std::sqrt(discrim);

        // Calculate the two solutions to the quadratic equation
        const auto d1 = -(b / 2.0f) + rootDiscrim;
        const auto d2 = -(b / 2.0f) - rootDiscrim;

        // and choose the one closest to the ray origin
        ray.e = ray.o + std::min(d1, d2) * d;

        return true;
    }

    float Sphere::area() const {
        return static_cast<float>(4.0f * M_PI * r*r);
    }
}