//
// Created by Niclas Olmenius on 2017-09-07.
//

#include "triangle.h"
#include "ray.h"
#include "intersectioninfo.h"
#include "options.h"
#include <glm/gtc/matrix_inverse.hpp>

namespace rays {

    bool Triangle::intersect(const Ray &ray, IntersectionInfo *isect, float *tHit) const {


//        const float EPSILON = 0.00001;
        auto v0 = V[0];
        auto v1 = V[1];
        auto v2 = V[2];
//
//        glm::vec3 edge1, edge2, h, s, q;
//        float a,f,u,v;
//        edge1 = v1 - v0;
//        edge2 = v2 - v0;
//        h = cross(ray.d,edge2);
//        a = dot(edge1, h);
//        if (a > -EPSILON && a < EPSILON)
//            return false;
//        f = 1/a;
//        s = ray.o - v0;
//        u = f * (dot(s,h));
//        if (u < 0.0 || u > 1.0)
//            return false;
//        q = cross(s, edge1);
//        v = f * dot(ray.d, q);
//        if (v < 0.0 || u + v > 1.0)
//            return false;
//        // At this stage we can compute t to find out where the intersection point is on the line.
//        float t = f * dot(edge2, q);
//        if (t > EPSILON && t < ray.tMax) // ray intersection
//        {
//        auto hitPoint = ray.o + t * ray.d;
//            *isect = IntersectionInfo(hitPoint, -ray.d, normal, this);
//
//            return true;
//        }
//        else // This means that there is a line intersection but not a ray intersection.
//            return false;


//        const auto &v0 = getV0();
//        const auto &v1 = getV1();
//        const auto &v2 = getV2();

//        const auto e1 = v1 - v0;
//        const auto e2 = v2 - v0;
//        const auto dir = ray.d;
//        const auto P = cross(dir, e2);
//
//        double det = dot(e1,P);
//
//        const float epsilon = std::numeric_limits<float>::epsilon();
//        if (det < epsilon && det > -epsilon) {
//            return false;
//        }
//        // Test bounds for u
//        const glm::vec3 T = ray.o - v0;
//        double u = dot(T, P);
//        if (u < 0.0 || u > det) {
//            return false;
//        }
//
//        // Test bounds for v
//        const auto Q = cross(T, e1);
//        double v = dot(dir, Q);
//        if (v < 0.0 || u + v > det) {
//            return false;
//        }
//
//        // We have a match!
//        // Calculate t
//        float t = dot(Q, e2);
//        double invDet = 1.0 / det;
//        t *= invDet;
//        if (t > ray.tMax || t <= epsilon) { return false; } // Bailout if intersection point is further away
////        u *= invDet;
////        v *= invDet;
//
//        *tHit = t;
//        auto end = ray.o + t * dir;
//
//        *isect = IntersectionInfo(end, -ray.d, normal, this);
//
//        return true;

//        glm::vec4 tO = T * glm::vec4(ray.o, 1);
        glm::vec3 tO = T * (ray.o - V[0]);
        glm::vec3 tDir = T * ray.d;

        float t = -tO.z / tDir.z;
        if (t < -options::epsilon || t > ray.tMax) return false;
        float u = tO.x + t * tDir.x;
        if (u < -options::epsilon) { return false; }
        float v = tO.y + t * tDir.y;
        if (v < -options::epsilon || u + v > 1) { return false; }

        *tHit = t;
        auto hitPoint = ray.o + t * ray.d;

        isect->p = hitPoint;
        isect->wo = -ray.d;
        isect->n = normal;
        isect->shape = this;
//        = IntersectionInfo(hitPoint, -ray.d, normal, this);

        return true;


    }

    float Triangle::area() const {
        return 0.5f * (glm::cross((V[1] - V[0]), (V[2] - V[0]))).length();
    }

    glm::vec3 Triangle::getRandomPoint(glm::vec2 r2) const {

        // pbrt
        float su0 = glm::sqrt(r2[0]);

        float u = 1 - su0;
        float v = r2[1] * su0;

        return u * V[0] + v * V[1] + (1 - u - v) * V[2];
    }

    glm::mat3 Triangle::calculateAffineTransformation() {
        auto AB = V[1] - V[0];
        auto AC = V[2] - V[0];
        auto A = V[0];

        glm::mat3 affineTMat;
        affineTMat[0] = glm::vec3(AB);
        affineTMat[1] = glm::vec3(AC);
        affineTMat[2] = glm::vec3(normal);
//        affineTMat[3] = glm::vec4(A, 1);
        affineTMat = glm::inverse(affineTMat);

        return affineTMat;
    }


}
