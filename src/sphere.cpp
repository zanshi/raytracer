//
// Created by Niclas Olmenius on 2017-09-15.
//

#include "sphere.h"
#include "ray.h"
#include "intersectioninfo.h"

namespace rays {

    bool Sphere::intersect(Ray &ray, IntersectionInfo *isect, float *tHit) const {
        const Vector3f d = ray.d;
        const float a = d.dot(d);
        const float b = 2.0f * (d.dot(ray.o - center));
        const float c = (ray.o - center).dot(ray.o - center) - (r * r);

        const auto discrim = std::pow((b / 2.0f), 2) - a * c;

        if (discrim < 0) { return false; } // No real solutions
        const float rootDiscrim = sqrtf(discrim);

        // Calculate the two solutions to the quadratic equation
        const float bHalf = -(b * 0.5f);
        const auto d1 = bHalf + rootDiscrim;
        const auto d2 = bHalf - rootDiscrim;

        const float minD = std::min(d1, d2);

        if (minD > ray.t) { return false; }

        // and choose the one closest to the ray origin
        //ray.e = ray.o + std::min(d1, d2) * d;

        Vertex3 end = ray.o + minD * d;

        *isect = IntersectionInfo(end, ray.o - end, end - center, this);

        return true;
    }

    float Sphere::area() const {
        return static_cast<float>(4.0f * M_PI * r * r);
    }
}