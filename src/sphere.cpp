//
// Created by Niclas Olmenius on 2017-09-15.
//

#include "sphere.h"
#include "ray.h"
#include "intersectioninfo.h"

namespace rays {

    bool Sphere::intersect(const Ray &ray, IntersectionInfo *isect, float *tHit) const {

        float t0, t1;

        const glm::vec3 d = ray.d;
        const glm::vec3 L = ray.o - center;
        const float a = dot(d, d);
        const float b = 2.0f * (dot(d,L));
        const float c = dot(L, L) - (r * r);

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

        glm::vec3 end = ray.o + t0 * d;

        *isect = IntersectionInfo(end, -ray.d, glm::normalize(end - center), this);

        return true;
    }

    glm::vec3 Sphere::getRandomPoint(RNG &rng) const {
        return glm::vec3(); // not necessary
    }

    float Sphere::area() const {
        return static_cast<float>(4.0f * PI * r * r);
    }
}