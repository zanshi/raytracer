//
// Created by Niclas Olmenius on 2017-09-07.
//

#include "triangle.h"
#include "ray.h"
#include "intersectioninfo.h"

namespace rays {

    bool Triangle::intersect(Ray &ray, IntersectionInfo *isect, float *tHit) const {
        // Möller-Trumbore
        // Based on the canonical implementation
        const auto &v0 = getV0();
        const auto &v1 = getV1();
        const auto &v2 = getV2();

        const auto e1 = v1 - v0;
        const auto e2 = v2 - v0;
        const auto dir = ray.d;
        const auto P = dir.cross(e2);

        // Use determinant to bail out early in case the triangle is facing away
        double det = e1.dot(P);

        const float epsilon = std::numeric_limits<float>::epsilon();
        if (det < epsilon && det > -epsilon) {
            return false;
        }
        // Test bounds for u
        const Vector3f T = ray.o - v0;
        double u = T.dot(P);
        if (u < 0.0 || u > det) {
            return false;
        }

        // Test bounds for v
        const auto Q = T.cross(e1);
        double v = dir.dot(Q);
        if (v < 0.0 || u + v > det) {
            return false;
        }

        // We have a match!
        // Calculate t
        float t = Q.dot(e2);
        double invDet = 1.0 / det;
        t *= invDet;
        if (t > ray.tMax || t <= epsilon) { return false; } // Bailout if intersection point is further away
//        u *= invDet;
//        v *= invDet;

        *tHit = t;
        Vertex3f end = ray.o + t * dir;

        *isect = IntersectionInfo(end, -ray.d, normal, this);

        return true;
    }

    float Triangle::area() const {
        const auto &v0 = getV0();
        const auto &v1 = getV1();
        const auto &v2 = getV2();
        return 0.5f * ((v1 - v0).cross(v2 - v0)).length();
    }

    const Vertex3f &Triangle::getV0() const {
        return mesh->V[indices[0]];
    }

    const Vertex3f &Triangle::getV1() const {
        return mesh->V[indices[1]];
    }

    const Vertex3f &Triangle::getV2() const {
        return mesh->V[indices[2]];
    }

    Vertex3f Triangle::getRandomPoint(RNG &rng) const {
        float u, v;
        do {
            u = rng.getUniform1D();
            v = rng.getUniform1D();
        } while (u + v >= 1);

        return (1 - u - v) * getV0() + u * getV1() + v * getV2();
    }


}
