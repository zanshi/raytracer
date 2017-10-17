//
// Created by Niclas Olmenius on 2017-09-20.
//

#ifndef RAYTRACER_BSDF_H
#define RAYTRACER_BSDF_H

#include "colordbl.h"
#include "vector.h"

namespace rays {

    // From pbrt
    enum BSDF_Type {
        BSDF_DIFFUSE, // For Lambertian and Oren-Nayar
        BSDF_SPECULAR,  // For mirrors?
        BSDF_TRANSPARENT // Glass etc
    };

    const float AIR_INDEX = 1.0f;

    inline float absDot(const Vector3f &v0, const Vector3f &v1) {
        return std::abs(v0.dot(v1));
    }

    inline float cosTheta(const Vector3f& v) {
        return v.z;
    }

    inline float absCosTheta(const Vector3f& v) {
        return std::abs(v.z);
    }

    inline Vector3f reflect(const Vector3f &wo, const Vector3f &n) {
        return -wo + 2 * n.dot(wo) * n;
    }

    inline bool refract(Vector3f *wt, const Vector3f &wi, const Vector3f &n, float index) {

        float cosThetaI = clamp(n.dot(wi), -1.0f, 1.0f);

        float n1 = AIR_INDEX;
        float n2 = index;
        Vector3f N = n;

        bool entering = cosThetaI > 0.0f;
        if (!entering) {
            float temp = n1;
            n1 = n2;
            n2 = temp;
            N = -N;
//            cosThetaI = std::abs(cosThetaI);
        } else {
//            cosThetaI = -cosThetaI;
        }

        float eta = n1 / n2;
        float sinThetaI = std::max(0.0f, 1 - cosThetaI * cosThetaI);
        float sinThetaT = eta * eta * sinThetaI;

        if (sinThetaT >= 1) { return false; } // Total reflection

        float cosThetaT = std::sqrt(1 - sinThetaT);

//        *wt = eta * wi + n * (-eta * cosThetaI - cosThetaT);
        *wt = eta * -wi + (eta * cosThetaI - cosThetaT) * N;

        return true;

    }

    // Calculate Dielectric-Dielectric reflection coefficient
    inline float fresnel(float cosThetaI, float index) {
        cosThetaI = clamp(cosThetaI, -1.0f, 1.0f);
        float n1 = AIR_INDEX;
        float n2 = index;

        bool entering = cosThetaI > 0.0f;
        if (!entering) {
            float temp = n1;
            n1 = n2;
            n2 = temp;
            cosThetaI = std::abs(cosThetaI);
        }

        float sinThetaI = std::sqrt(std::max(0.0f, 1 - cosThetaI * cosThetaI));
        float sinThetaT = n1 / n2 * sinThetaI;

        // Handle total internal reflection
        if (sinThetaT >= 1) return 1;
        float cosThetaT = std::sqrt(std::max(0.0f, 1 - sinThetaT * sinThetaT));
        float Rs = ((n2 * cosThetaI) - (n1 * cosThetaT)) /
                   ((n2 * cosThetaI) + (n1 * cosThetaT));
        float Rp = ((n1 * cosThetaI) - (n2 * cosThetaT)) /
                   ((n1 * cosThetaI) + (n2 * cosThetaT));
        return (Rs * Rs + Rp * Rp) / 2;

    }



    inline Vector3f worldToLocal(const Vector3f &ss, const Vector3f &ts, const Vector3f &ns, const Vector3f &v) {
        return Vector3f(v.dot(ss), v.dot(ts), v.dot(ns));
    }

    inline Vector3f localToWorld(const Vector3f &ss, const Vector3f &ts, const Vector3f &ns, const Vector3f &v) {
        return Vector3f(ss.x * v.x + ts.x * v.y + ns.x * v.z,
                        ss.y * v.x + ts.y * v.y + ns.y * v.z,
                        ss.z * v.x + ts.z * v.y + ns.z * v.z);
    }


    struct BSDF {
        explicit BSDF(const ColorDbl &c, float index) : R(c), index(index) {}

        virtual ~BSDF() = default;

        virtual ColorDbl fr(Vector3f *wi, const Vector3f &wo) const = 0;

//        virtual ColorDbl ft(Vector3f *wi, const Vector3f &wo) const = 0;

        virtual float pdf(const Vector3f &wi, const Vector3f &wo) const {
            return cosTheta(wi) * invPI; // pdf for Lambertian and Oren-Nayar
        }

        virtual BSDF_Type getType() const = 0;

        const ColorDbl R;
        const float index;
    };

    struct Lambertian final : BSDF {

        explicit Lambertian(const ColorDbl &c, float index = 1) : BSDF(c, index) {}
        virtual ~Lambertian() = default;

        ColorDbl fr(Vector3f *wi, const Vector3f &wo) const override {
            return R * invPI;
        }

        BSDF_Type getType() const override {
            return BSDF_Type::BSDF_DIFFUSE;
        }

    };

    struct Glass final : BSDF {
        explicit Glass(const ColorDbl &c, float index) : BSDF(c, index) {}
        virtual ~Glass() = default;

        ColorDbl fr(Vector3f *wi, const Vector3f &wo) const override {
//            *wi = Vector3f(-wo.x, -wo.y, wo.z);
            return R;
        }

        BSDF_Type getType() const override {
            return BSDF_TRANSPARENT;
        }

    };

}

#endif //RAYTRACER_BSDF_H
