//
// Created by Niclas Olmenius on 2017-09-15.
//

#include "sphere.h"
#include "ray.h"
#include "intersectioninfo.h"

namespace rays {

    bool Sphere::intersect(Ray &ray, IntersectionInfo *isect, float *tHit) const {

        float t0, t1;

        const Vector3f d = ray.d;
        const Vector3f L = ray.o - center;
        const float a = d.dot(d);
        const float b = 2.0f * (d.dot(L));
        const float c = L.dot(L) - (r * r);

//        const float discrim = std::pow((b / 2.0f), 2) - a * c;
        const auto discrim = b * b - 4 * a * c;

        if (discrim < 0) {
            return false;
        } else if (discrim == 0) {
            t0 = t1 = -0.5f * b / a;
        } else {
            const float rootDiscrim = std::sqrt(discrim);
            float q = (b > 0) ?
                      -0.5f * (b + rootDiscrim) :
                      -0.5f * (b - rootDiscrim);
            t0 = q / a;
            t1 = c / q;
        }

        if (t0 > t1) {
            float temp = t1;
            t1 = t0;
            t0 = temp;
        }

        if (t0 < 0) {
            t0 = t1; // if t0 is negative, let's use t1 instead
            if (t0 < 0) return false; // both t0 and t1 are negative
        }


        if (t0 > ray.tMax) {
            return false;
        }

//        // No real solutions
//
//        // Calculate the two solutions to the quadratic equation
//        const float bHalf = -(b * 0.5f);
//        const float d1 = bHalf + rootDiscrim;
//        const float d2 = bHalf - rootDiscrim;
//
//        const float minD = std::min(d1, d2);
//
//        if (minD < 0) { return false; }
//
//        if (minD > *tHit) { return false; }

        // and choose the one closest to the ray origin
        //ray.e = ray.o + std::min(d1, d2) * d;

        *tHit = t0;

        Vertex3f end = ray.o + t0 * d;

        *isect = IntersectionInfo(end, -ray.d, normalize(end - center), this);

        return true;
    }

    Vertex3f Sphere::getRandomPoint(RNG &rng) const {
        return Vertex3f(); // not necessary
    }

    float Sphere::area() const {
        return static_cast<float>(4.0f * M_PI * r * r);
    }
}