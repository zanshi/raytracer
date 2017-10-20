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

    inline void coordinateSystem(const Vector3f &v1, Vector3f *v2,
                                 Vector3f *v3) {
        if (std::abs(v1.x) > std::abs(v1.y))
            *v2 = Vector3f(-v1.z, 0, v1.x) / std::sqrt(v1.x * v1.x + v1.z * v1.z);
        else
            *v2 = Vector3f(0, v1.z, -v1.y) / std::sqrt(v1.y * v1.y + v1.z * v1.z);
        *v3 = cross(v1,*v2);
    }

    inline float absDot(const Vector3f &v0, const Vector3f &v1) {
        return std::abs(dot(v0, v1));
    }

    inline float cosTheta(const Vector3f &v) {
        return v.z;
    }

    inline float Cos2Theta(const Vector3f &w) { return w.z * w.z; }


    inline float absCosTheta(const Vector3f &v) {
        return std::abs(v.z);
    }

    inline float Sin2Theta(const Vector3f &w) {
        return std::max((float) 0, (float) 1 - Cos2Theta(w));
    }

    inline float SinTheta(const Vector3f &w) { return std::sqrt(Sin2Theta(w)); }

    inline float TanTheta(const Vector3f &w) { return SinTheta(w) / cosTheta(w); }

    inline float Tan2Theta(const Vector3f &w) {
        return Sin2Theta(w) / Cos2Theta(w);
    }

    inline float CosPhi(const Vector3f &w) {
        float sinTheta = SinTheta(w);
        return (sinTheta == 0) ? 1 : clamp(w.x / sinTheta, -1.f, 1.f);
    }

    inline float SinPhi(const Vector3f &w) {
        float sinTheta = SinTheta(w);
        return (sinTheta == 0) ? 0 : clamp(w.y / sinTheta, -1.f, 1.f);
    }

    inline float Cos2Phi(const Vector3f &w) { return CosPhi(w) * CosPhi(w); }

    inline float Sin2Phi(const Vector3f &w) { return SinPhi(w) * SinPhi(w); }

    inline Vector3f reflect(const Vector3f &wo, const Vector3f &n) {
        return wo - 2 * dot(n,wo) * n;
    }

    inline bool refract(Vector3f *wt, const Vector3f &wi, const Vector3f &n, float index) {

        float cosThetaI = clamp(dot(n,wi), -1.0f, 1.0f);

        float n1 = AIR_INDEX;
        float n2 = index;
        Vector3f N = n;

//        bool entering = cosThetaI > 0.0f;
//        if (!entering) {
//            float temp = n1;
//            n1 = n2;
//            n2 = temp;
//            N = -N;
////            cosThetaI = std::abs(cosThetaI);
//        }
        if (cosThetaI < 0) { cosThetaI = -cosThetaI; }
        else {
            std::swap(n1, n2);
            N = -N;
        }


        float eta = n1 / n2;

        float k = 1 - eta * eta * (1 - cosThetaI * cosThetaI);
        if (k < 0) { return false; }

        *wt = eta * wi + (eta * cosThetaI - std::sqrt(k)) * n;

        return true;

    }

    // Calculate Dielectric-Dielectric reflection coefficient
    inline float fresnel(float cosThetaI, float index) {
        cosThetaI = clamp(cosThetaI, -1.0f, 1.0f);
        float n1 = AIR_INDEX;
        float n2 = index;

        if (cosThetaI > 0) { std::swap(n1, n2); }

        float sinThetaI = std::sqrt(std::max(0.0f, 1 - cosThetaI * cosThetaI));
        float sinThetaT = n1 / n2 * sinThetaI;

        // Handle total internal reflection
        if (sinThetaT >= 1) return 1;
        float cosThetaT = std::sqrt(std::max(0.0f, 1 - sinThetaT * sinThetaT));
        cosThetaI = std::abs(cosThetaI);
        float Rs = ((n2 * cosThetaI) - (n1 * cosThetaT)) / ((n2 * cosThetaI) + (n1 * cosThetaT));
        float Rp = ((n1 * cosThetaI) - (n2 * cosThetaT)) / ((n1 * cosThetaI) + (n2 * cosThetaT));
        return (Rs * Rs + Rp * Rp) / 2;

    }


    inline Vector3f worldToLocal(const Vector3f &ss, const Vector3f &ts, const Vector3f &ns, const Vector3f &v) {
        return Vector3f(dot(v,ss), dot(v,ts), dot(v,ns));
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

        virtual float pdf(const Vector3f &wi, const Vector3f &n) const {
            return absDot(wi, n) * invPI; // pdf for Lambertian and Oren-Nayar
        }

        virtual BSDF_Type getType() const = 0;

        const ColorDbl R;
        const float index;
    };

    struct Lambertian final : BSDF {

        explicit Lambertian(const ColorDbl &c, float index = 1) : BSDF(c, index) {}

        ~Lambertian() override = default;

        ColorDbl fr(Vector3f *wi, const Vector3f &wo) const override {
            return R * invPI;
        }

        BSDF_Type getType() const override {
            return BSDF_Type::BSDF_DIFFUSE;
        }

    };

    struct OrenNayar final : BSDF {

        explicit OrenNayar(const ColorDbl &c, float sigma, float index = 1) :
                BSDF(c, index), A(1 - ((0.5f * sigma * sigma) / (sigma * sigma + 0.33f))),
                B(0.45f * (sigma * sigma) / (sigma * sigma + 0.09f)) {}

        ~OrenNayar() override = default;

        ColorDbl fr(Vector3f *wi, const Vector3f &wo) const override {
            float sinThetaI = SinTheta(*wi);
            float sinThetaO = SinTheta(wo);
            // Compute cosine term of Oren-Nayar model
            float maxCos = 0;
            if (sinThetaI > 1e-4 && sinThetaO > 1e-4) {
                float sinPhiI = SinPhi(*wi), cosPhiI = CosPhi(*wi);
                float sinPhiO = SinPhi(wo), cosPhiO = CosPhi(wo);
                float dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
                maxCos = std::max((float) 0, dCos);
            }

            // Compute sine and tangent terms of Oren-Nayar model
            float sinAlpha, tanBeta;
            if (absCosTheta(*wi) > absCosTheta(wo)) {
                sinAlpha = sinThetaO;
                tanBeta = sinThetaI / absCosTheta(*wi);
            } else {
                sinAlpha = sinThetaI;
                tanBeta = sinThetaO / absCosTheta(wo);
            }
            return R * invPI * (A + B * maxCos * sinAlpha * tanBeta);
        }

        BSDF_Type getType() const override {
            return BSDF_Type::BSDF_DIFFUSE;
        }

        float A, B;

    };

    struct Glass final : BSDF {
        explicit Glass(const ColorDbl &c, float index) : BSDF(c, index) {}

        ~Glass() override = default;

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
