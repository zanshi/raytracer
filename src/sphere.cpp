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
//        const glm::vec3 L = ray.o - center;
//        const float a = glm::dot(d, d);
//        const float b = 2.0f * (glm::dot(d,L));
//        const float c = glm::dot(L, L) - (r * r);
//
//        const auto discrim = b * b - 4 * a * c;
//
//        const float epsilon = 1e-5f;
//
//        if (discrim < -epsilon) {
//            return false;
//        } else if (discrim < epsilon && discrim > -epsilon) {
//            t0 = t1 = -0.5f * b / a;
//        } else {
//            const float rootDiscrim = glm::sqrt(discrim);
//            float q = (b > 0) ?
//                      -0.5f * (b + rootDiscrim) :
//                      -0.5f * (b - rootDiscrim);
//            t0 = q / a;
//            t1 = c / q;
//        }
//
//        if (t0 > t1) {
//            float temp = t1;
//            t1 = t0;
//            t0 = temp;
//        }
//
//        if (t0 < -epsilon) {
//            t0 = t1; // if t0 is negative, let's use t1 instead
//            if (t0 < 0) return false; // both t0 and t1 are negative
//        }

        glm::vec3 L = center - ray.o;
        float tc = glm::dot(L, ray.d);
        if (tc < 0) { return false; }
        float d2 = glm::dot(L, L) - (tc * tc);
        float r2 = r * r;
        if (d2 > r2) { return false;}
        float t1c = glm::sqrt(r2 - d2);

        t0 = tc - t1c;
        t1 = tc + t1c;
        t0 = glm::min(t0, t1);

        if (t0 < 0) {
            t0 = t1;
            if (t0 < 0) { return false; }
        }

        if (t0 > ray.tMax) {
            return false;
        }

        *tHit = t0;

        glm::vec3 end = ray.o + t0 * d;

        *isect = IntersectionInfo(end, -ray.d, glm::normalize(end - center), this);

        return true;
    }

    glm::vec3 Sphere::getRandomPoint(glm::vec2 r2) const {
        return glm::vec3(); // not necessary
    }

    float Sphere::area() const {
        return static_cast<float>(4.0f * PI * r * r);
    }
}