//
// Created by Niclas Olmenius on 2017-09-07.
//

#include "triangle.h"
#include "ray.h"
#include "vertex.h"
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

//#ifdef TEST_CULL
        if (det < std::numeric_limits<float>::epsilon()) {
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
        if (t > *tHit) { return false; } // Bailout if intersection point is further away
        double invDet = 1.0 / det;
        t *= invDet;
        u *= invDet;
        v *= invDet;

        Vertex3 end = ray.o + t * dir;

        *isect = IntersectionInfo(end, ray.o - end, normal, this);

//#endif
        return true;
    }

    float Triangle::area() const {
        const auto &v0 = getV0();
        const auto &v1 = getV1();
        const auto &v2 = getV2();
        return 0.5f * ((v1 - v0).cross(v2 - v0)).length();
    }

    const Vertex3f &Triangle::getV0() const {
        return mesh->V->operator[](indices[0]);
    }

    const Vertex3f &Triangle::getV1() const {
        return mesh->V->operator[](indices[1]);
    }

    const Vertex3f &Triangle::getV2() const {
        return mesh->V->operator[](indices[2]);
    }

}
