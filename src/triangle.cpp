//
// Created by Niclas Olmenius on 2017-09-07.
//

#include "triangle.h"
#include "ray.h"
#include "intersectioninfo.h"

#include <glm/gtx/intersect.hpp>

namespace rays {

    bool Triangle::intersect(Ray &ray, IntersectionInfo *isect, float *tHit) const {
        // Möller-Trumbore
        // Based on the canonical implementation
        const auto &v0 = getV0();
        const auto &v1 = getV1();
        const auto &v2 = getV2();
//        const auto &d = ray.d;

        glm::vec3 p0t = v0 - ray.o;
        glm::vec3 p1t = v1 - ray.o;
        glm::vec3 p2t = v2 - ray.o;

        int kz = maxDimension(abs(ray.d));
        int kx = kz + 1;
        if (kx == 3) kx = 0;
        int ky = kx + 1;
        if (ky == 3) ky = 0;
        glm::vec3 d = permute(ray.d, kx, ky, kz);
        p0t = permute(p0t, kx, ky, kz);
        p1t = permute(p1t, kx, ky, kz);
        p2t = permute(p2t, kx, ky, kz);


        float Sx = -d.x / d.z;
        float Sy = -d.y / d.z;
        float Sz = 1.0f / d.z;

        p0t.x += Sx * p0t.z;
        p0t.y += Sy * p0t.z;
        p1t.x += Sx * p1t.z;
        p1t.y += Sy * p1t.z;
        p2t.x += Sx * p2t.z;
        p2t.y += Sy * p2t.z;

        float e0 = p1t.x * p2t.y - p1t.y * p2t.x;
        float e1 = p2t.x * p0t.y - p2t.y * p0t.x;
        float e2 = p0t.x * p1t.y - p0t.y * p1t.x;


        if ((e0 < 0 || e1 < 0 || e2 < 0) && (e0 > 0 || e1 > 0 || e2 > 0))
            return false;
        float det = e0 + e1 + e2;
        if (det == 0)
            return false;

        p0t.z *= Sz;
        p1t.z *= Sz;
        p2t.z *= Sz;
        float tScaled = e0 * p0t.z + e1 * p1t.z + e2 * p2t.z;

        if (det < 0 && (tScaled >= 0 || tScaled < ray.tMax * det))
            return false;
        else if (det > 0 && (tScaled <= 0 || tScaled > ray.tMax * det))
            return false;

        float invDet = 1 / det;
        float b0 = e0 * invDet;
        float b1 = e1 * invDet;
        float b2 = e2 * invDet;
        float t = tScaled * invDet;

        glm::vec3 hitPoint = b0 * v0 + b1 * v1 + b2 * v2;

        *tHit = t;
        *isect = IntersectionInfo(hitPoint, -ray.d, normal, this);


        return true;



//        const auto e1 = v1 - v0;
//        const auto e2 = v2 - v0;
//        const auto dir = ray.d;
//        const auto P = cross(dir, e2);
//
//        // Use determinant to bail out early in case the triangle is facing away
//        float det = dot(e1, P);
//
//        Vector3f Q;
//
//        const float epsilon = std::numeric_limits<float>::epsilon();
////        const float epsilon = 1e-5f;
//
//        if (det > epsilon) {
//            const Vector3f T = ray.o - v0;
//            float u = dot(T, P);
//
//            if (u < 0.0 || u > det) {
//                return false;
//            }
//
//            // Test bounds for v
//            Q = cross(T, e1);
//            float v = dot(dir, Q);
//            if (v < 0.0 || u + v > det) {
//                return false;
//            }
//
//        } else if (det < -epsilon) {
//            // Test bounds for u
//            const Vector3f T = ray.o - v0;
//            float u = dot(T, P);
//            if (u > 0.0 || u < det) {
//                return false;
//            }
//            Q = cross(T, e1);
//            float v = dot(dir, Q);
//            if (v > 0.0 || u + v < det) {
//                return false;
//            }
//
//        } else { return false; }
//
//        // We have a match!
//        // Calculate t
//        float invDet = 1.0f / det;
//
//        float t = dot(e2, Q) * invDet;
//        if (t > ray.tMax) { return false; } // Bailout if intersection point is further away
//
//        *tHit = t;
//        Vertex3f end = ray.o + t * dir;
//
//        *isect = IntersectionInfo(end, -ray.d, normal, this);
//
//        return true;


    }

    float Triangle::area() const {
        const auto &v0 = getV0();
        const auto &v1 = getV1();
        const auto &v2 = getV2();
        return 0.5f * (cross((v1 - v0), (v2 - v0))).length();
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
