//
// Created by Niclas Olmenius on 2017-09-20.
//

#ifndef RAYTRACER_BSDF_H
#define RAYTRACER_BSDF_H

#include "colordbl.h"
#include <glm/glm.hpp>

namespace rays {

    // From pbrt
    enum BSDF_Type {
        BSDF_DIFFUSE, // For Lambertian and Oren-Nayar
        BSDF_SPECULAR,  // For mirrors?
        BSDF_TRANSPARENT // Glass etc
    };

    inline void coordinateSystem(const glm::vec3 &v1, glm::vec3 *v2,
                                 glm::vec3 *v3) {
        if (std::abs(v1.x) > std::abs(v1.y))
            *v2 = glm::vec3(-v1.z, 0, v1.x) / std::sqrt(v1.x * v1.x + v1.z * v1.z);
        else
            *v2 = glm::vec3(0, v1.z, -v1.y) / std::sqrt(v1.y * v1.y + v1.z * v1.z);
        *v3 = cross(v1, *v2);
    }

    inline float absDot(const glm::vec3 &v0, const glm::vec3 &v1) {
        return glm::abs(glm::dot(v0, v1));
    }

    inline float cosTheta(const glm::vec3 &v) {
        return v.z;
    }

    inline float Cos2Theta(const glm::vec3 &w) { return w.z * w.z; }


    inline float absCosTheta(const glm::vec3 &v) {
        return std::abs(v.z);
    }

    inline float Sin2Theta(const glm::vec3 &w) {
        return std::max((float) 0, (float) 1 - Cos2Theta(w));
    }

    inline float SinTheta(const glm::vec3 &w) { return std::sqrt(Sin2Theta(w)); }

    inline float CosPhi(const glm::vec3 &w) {
        float sinTheta = SinTheta(w);
        return (sinTheta == 0) ? 1 : clamp(w.x / sinTheta, -1.f, 1.f);
    }

    inline float SinPhi(const glm::vec3 &w) {
        float sinTheta = SinTheta(w);
        return (sinTheta == 0) ? 0 : clamp(w.y / sinTheta, -1.f, 1.f);
    }


    inline float balanceHeuristic(float numerator, float denominator) {
        return numerator / (numerator + denominator);
    }

    inline glm::vec3 reflect(const glm::vec3 &wo, const glm::vec3 &n) {
        return wo - 2.0f * glm::dot(n, wo) * n;
    }

    inline bool refract(glm::vec3 *wt, const glm::vec3 &wi, const glm::vec3 &n, float invEta) {

        const float cosThetaI = glm::clamp(glm::dot(n, wi), -1.0f, 1.0f);


        const float k = 1.0f - invEta * invEta * (1 - cosThetaI * cosThetaI);
        if (k < 0.0f) { return false; }

        *wt = glm::normalize(invEta * wi + (-invEta * cosThetaI - glm::sqrt(k)) * n);

        return true;

    }

    // Calculate Dielectric-Dielectric reflection coefficient
    inline float fresnel(float cosThetaI, float eta) {
        // Schlick
        float etaMinus = eta - 1;
        float etaPlus = eta + 1;

        float R0 = (etaMinus * etaMinus) / (etaPlus * etaPlus);

        return R0 + (1.0f - R0) * glm::pow(1.0f - cosThetaI, 5.0f);

    }

    inline glm::vec3 sphericalToCartesian(float theta, float phi) {
        return glm::vec3{theta * glm::sin(phi), theta * glm::cos(phi), glm::sqrt(1 - theta * theta)};
    }

    inline glm::vec3 worldToLocal(const glm::vec3 &ss, const glm::vec3 &ts, const glm::vec3 &ns, const glm::vec3 &v) {
        return glm::vec3(glm::dot(v, ss), glm::dot(v, ts), glm::dot(v, ns));
    }

    inline glm::vec3 localToWorld(const glm::vec3 &ss, const glm::vec3 &ts, const glm::vec3 &ns, const glm::vec3 &v) {
        return glm::vec3(ss.x * v.x + ts.x * v.y + ns.x * v.z,
                         ss.y * v.x + ts.y * v.y + ns.y * v.z,
                         ss.z * v.x + ts.z * v.y + ns.z * v.z);
    }


    struct BSDF {
        explicit BSDF(const ColorDbl &c, float index) : R(c), index(index) {}

        virtual ~BSDF() = default;

        virtual ColorDbl sample(const glm::vec3 &wi, const glm::vec3 &wo) const = 0;

        virtual float pdf(const glm::vec3 &wi, const glm::vec3 &wo) const {
            return wi.z * wo.z > 0 ? absCosTheta(wi) * invPI : 0;
        };

        virtual BSDF_Type getType() const = 0;

        const ColorDbl R;
        const float index;
    };

    struct Lambertian final : BSDF {

        explicit Lambertian(const ColorDbl &c, float index = 1) : BSDF(c, index) {}

        ~Lambertian() override = default;

        ColorDbl sample(const glm::vec3 &wi, const glm::vec3 &wo) const override {
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

        ColorDbl sample(const glm::vec3 &wi, const glm::vec3 &wo) const override {
            float sinThetaI = SinTheta(wi);
            float sinThetaO = SinTheta(wo);
            // Compute cosine term of Oren-Nayar model
            float maxCos = 0;
            if (sinThetaI > 1e-4 && sinThetaO > 1e-4) {
                float sinPhiI = SinPhi(wi), cosPhiI = CosPhi(wi);
                float sinPhiO = SinPhi(wo), cosPhiO = CosPhi(wo);
                float dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
                maxCos = std::max((float) 0, dCos);
            }

            // Compute sine and tangent terms of Oren-Nayar model
            float sinAlpha, tanBeta;
            if (absCosTheta(wi) > absCosTheta(wo)) {
                sinAlpha = sinThetaO;
                tanBeta = sinThetaI / absCosTheta(wi);
            } else {
                sinAlpha = sinThetaI;
                tanBeta = sinThetaO / absCosTheta(wo);
            }
            return R * invPI * (A + B * maxCos * sinAlpha * tanBeta);
        }

        BSDF_Type getType() const override {
            return BSDF_Type::BSDF_DIFFUSE;
        }

        const float A, B;

    };

    struct Glass final : BSDF {
        explicit Glass(const ColorDbl &c, float index) : BSDF(c, index) {}

        ~Glass() override = default;

        ColorDbl sample(const glm::vec3 &wi, const glm::vec3 &wo) const override {
            return R;
        }

        float pdf(const glm::vec3 &wi, const glm::vec3 &wo) const override {
            return 1;
        }

        BSDF_Type getType() const override {
            return BSDF_TRANSPARENT;
        }

    };

    struct Mirror final : BSDF {
        explicit Mirror(const ColorDbl &c, float index) : BSDF(c, index) {}

        ~Mirror() override = default;

        ColorDbl sample(const glm::vec3 &wi, const glm::vec3 &wo) const override {
            return R;
        }

        float pdf(const glm::vec3 &wi, const glm::vec3 &wo) const override {
            return 1;
        }

        BSDF_Type getType() const override {
            return BSDF_SPECULAR;
        }

    };

}

#endif //RAYTRACER_BSDF_H
