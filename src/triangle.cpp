//
// Created by Niclas Olmenius on 2017-09-07.
//

#include "triangle.h"
#include "ray.h"
#include "intersectioninfo.h"
#include <glm/gtc/matrix_inverse.hpp>

namespace rays {

    bool Triangle::intersect(const Ray &ray, IntersectionInfo *isect, float *tHit) const {
//		if (glm::dot(ray.d, normal) > 0) { return false; }
        // Möller-Trumbore
        // Based on the canonical implementation

//        glm::vec3 v0 = V[0];
//        glm::vec3 v1 = V[1];
//        glm::vec3 v2 = V[2];
//
//        glm::vec3 p0t = v0 - ray.o;
//        glm::vec3 p1t = v1 - ray.o;
//        glm::vec3 p2t = v2 - ray.o;
//
//        int kz = maxDimension(abs(ray.d));
//        int kx = kz + 1;
//        if (kx == 3) kx = 0;
//        int ky = kx + 1;
//        if (ky == 3) ky = 0;
//        glm::vec3 d = permute(ray.d, kx, ky, kz);
//        p0t = permute(p0t, kx, ky, kz);
//        p1t = permute(p1t, kx, ky, kz);
//        p2t = permute(p2t, kx, ky, kz);
//
//
//        float Sx = -d.x / d.z;
//        float Sy = -d.y / d.z;
//        float Sz = 1.0f / d.z;
//
//        p0t.x += Sx * p0t.z;
//        p0t.y += Sy * p0t.z;
//        p1t.x += Sx * p1t.z;
//        p1t.y += Sy * p1t.z;
//        p2t.x += Sx * p2t.z;
//        p2t.y += Sy * p2t.z;
//
//        float e0 = p1t.x * p2t.y - p1t.y * p2t.x;
//        float e1 = p2t.x * p0t.y - p2t.y * p0t.x;
//        float e2 = p0t.x * p1t.y - p0t.y * p1t.x;
//
//
//        // Fall back to double precision test at triangle edges
////        if (e0 == 0.0f || e1 == 0.0f || e2 == 0.0f) {
////            double p2txp1ty = (double) p2t.x * (double) p1t.y;
////            double p2typ1tx = (double) p2t.y * (double) p1t.x;
////            e0 = (float) (p2typ1tx - p2txp1ty);
////            double p0txp2ty = (double) p0t.x * (double) p2t.y;
////            double p0typ2tx = (double) p0t.y * (double) p2t.x;
////            e1 = (float) (p0typ2tx - p0txp2ty);
////            double p1txp0ty = (double) p1t.x * (double) p0t.y;
////            double p1typ0tx = (double) p1t.y * (double) p0t.x;
////            e2 = (float) (p1typ0tx - p1txp0ty);
////        }
//
//        if ((e0 < 0 || e1 < 0 || e2 < 0) && (e0 > 0 || e1 > 0 || e2 > 0))
//            return false;
//        float det = e0 + e1 + e2;
//        if (det == 0)
//            return false;
//
//        p0t.z *= Sz;
//        p1t.z *= Sz;
//        p2t.z *= Sz;
//        float tScaled = e0 * p0t.z + e1 * p1t.z + e2 * p2t.z;
//
//        if (det < 0 && (tScaled >= 0 || tScaled < ray.tMax * det))
//            return false;
//        else if (det > 0 && (tScaled <= 0 || tScaled > ray.tMax * det))
//            return false;
//
//        float invDet = 1 / det;
//        float b0 = e0 * invDet;
//        float b1 = e1 * invDet;
//        float b2 = e2 * invDet;
//        float t = tScaled * invDet;
//
//        glm::vec3 hitPoint = b0 * v0 + b1 * v1 + b2 * v2;
//
//        *tHit = t;
//        *isect = IntersectionInfo(hitPoint, -ray.d, normal, this);
//
//
//        return true;

//        float EPSILON = std::numeric_limits<float>::epsilon();
//
//        glm::vec3 T = ray.o - V[0];
//        glm::vec3 E1 = V[1] - V[0];
//        glm::vec3 E2 = V[2] - V[0];
//        glm::vec3 D = ray.d;
//
//        glm::vec3 P = glm::cross(D, E2);
//        glm::vec3 Q = glm::cross(T, E1);
//
//        float det = glm::dot(P, E1);
//
//        if(det > -EPSILON && det < EPSILON)
//            return false;
//
//        float inverseDet = 1.0f/det;
//
//        float u = glm::dot(T, P) * inverseDet;
//        float v = glm::dot(D, Q) * inverseDet;
//
//        if(u < 0.0f || u > 1.0f) {
//            return false;
//        }
//
//        if(v < 0.0f || u + v > 1.0f) {
//            return false;
//        }
//
//        float t = glm::dot(Q, E2) * inverseDet;
//
//        if(t > EPSILON) {
//
//            *tHit = t;
//            glm::vec3 hitPoint = ray.o + t * ray.d;
//            *isect = IntersectionInfo(hitPoint, -ray.d, normal, this);
//            return true;
//        }
//
//        return false;

        // Möller Trumbore

//        float epsilon = 1e-05;
//
//        glm::vec3 e1 = V[1] - V[0];
//        glm::vec3 e2 = V[2] - V[0];
//        glm::vec3 P = glm::cross(ray.d, e2);
//        float det = glm::dot(e1, P);
//
//
//        if (glm::abs(det) < epsilon) {
//            return false;
//        }
//
//        float invDet = 1 / det;
//
//        auto T = ray.o - V[0];
//        auto u = glm::dot(T, P) * invDet;
//        if (u < 0 || u > 1) return false;
//
//        auto Q = glm::cross(T, e1);
//        auto v = glm::dot(ray.d, Q) * invDet;
//        if (v < 0 || u + v > 1) return false;
//
//        float t = glm::dot(e2, Q) * invDet;
//
//        if (t > ray.tMax || t < epsilon) { return false;}



        //

//        glm::vec3 tO = m * ray.o + V[0];
//        glm::vec3 tDir = m * ray.d;

        glm::vec4 tO = T * glm::vec4(ray.o, 1);
        glm::vec4 tDir = T * glm::vec4(ray.d, 0);

        float t = -tO.z / tDir.z;
        if (t < 0 || t > ray.tMax) return false;
        float u = tO.x + t * tDir.x;
        if (u < 0) { return false; }
        float v = tO.y + t * tDir.y;
        if (v < 0 || u + v > 1) { return false; }

        *tHit = t;
        auto hitPoint = ray.o + t * ray.d;

        *isect = IntersectionInfo(hitPoint, -ray.d, normal, this);

        return true;


    }

    float Triangle::area() const {
        return 0.5f * (glm::cross((V[1] - V[0]), (V[2] - V[0]))).length();
    }

    glm::vec3 Triangle::getRandomPoint(RNG &rng) const {
//        float u, v;
//        do {
//            u = rng.getUniform1D();
//            v = rng.getUniform1D();
//        } while (u + v >= 1);
//
//        return (1 - u - v) * V[0] + u * V[1] + v * V[2];

        // pbrt

        glm::vec2 r = rng.getUniform2D();
        float su0 = glm::sqrt(r[0]);

        float u = 1 - su0;
        float v = r[1] * su0;

        return (1 - u - v) * V[0] + u * V[1] + v * V[2];

    }

    glm::mat4 Triangle::calculateAffineTransformation() {
        auto AB = V[1] - V[0];
        auto AC = V[2] - V[0];
        auto A = V[0];

//            auto a = V[1];
//            auto b = V[2];
//            auto c = V[0];
//            auto n = normal;
//
//            glm::mat3 affine;

        // Inverse of T
//            affine[0] = glm::vec3(a.x - c.x, b.x - c.x, n.x);
//            affine[1] = glm::vec3(a.y - c.y, b.y - c.y, n.y);
//            affine[2] = glm::vec3(a.z - c.z, b.z - c.z, n.z);

//            affine[0] = a - c;
//            affine[1] = b - c;
//            affine[2] = n;
//
//            affine[0] = AB;
//            affine[1] = AC;
//            affine[2] = normal;
//
//            affine = glm::inverse(affine);
//
//            return affine;

//            glm::mat3 affine;
//            affine[0] = glm::vec3(AB);
//            affine[1] = glm::vec3(AC);
//            affine[2] = glm::vec3(normal);
//            affine = glm::inverse(affine);
//
//            return affine;

        glm::mat4 affineTMat;
        affineTMat[0] = glm::vec4(AB, 0);
        affineTMat[1] = glm::vec4(AC, 0);
        affineTMat[2] = glm::vec4(normal, 0);
        affineTMat[3] = glm::vec4(A, 1);
        affineTMat = glm::inverse(affineTMat);

        return affineTMat;
    }


}
