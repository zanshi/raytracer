//
// Created by Niclas Olmenius on 2017-09-06.
//

#include <glm/gtx/norm.hpp>
#include "camera.h"
#include "scene.h"
#include "ray.h"
#include "intersectioninfo.h"

namespace rays {

    ColorDbl Scene::trace(const Ray &ray, RNG &rng, int depth, bool specularBounce) const {

        ColorDbl L{0.0, 0.0, 0.0};

        if (depth == Options::maxDepth) {
//            std::cout << "Max depth reached. L = " << L << std::endl;
            return L;
        }

        // Check intersection
        IntersectionInfo isect;
        bool intersected = intersect(ray, &isect);

        if (!intersected) {
            std::cerr << "Ray escaped scene. " << ray << std::endl;
            return L;
        }

        // Intersected object is an area light source.
        // Add contribution from light and terminate.
        // We only care about the first bounce and specular reflections
        if (auto &&l = isect.obj->getAreaLight()) {
            if (depth == 0 || specularBounce) {
                L += l->L0 * l->intensity;
            }
            return L;
//            return l->L0;
        }

        glm::vec3 woWorld = isect.wo; // Should be normalized
        glm::vec3 n = isect.n; // Should be normalized
        const glm::vec3 hitPoint = isect.p;


        // Create local coordinate system
        glm::vec3 ss, ts;
        coordinateSystem(n, &ss, &ts);

        // Transform wo to local
        glm::vec3 wo = glm::normalize(worldToLocal(ss, ts, n, woWorld));

        // From here on, the calculations are in the local coordinate system of the
        // current hemisphere



        constexpr float epsilon = 1e-4f;

        // Check the intersection surface
        // Different behaviour depending on surface type
        if (isect.brdf->getType() == BSDF_DIFFUSE) {
            // Monte-Carlo: Use pdf and rng to calculate the reflected ray

            // Add direct light contribution
            // Send shadow rays to all light sources
            // For every area light

            // PDF = 1/Area
            // Draw u, v random numbers [0, 1[
            // if u + v < 1, create end point q_i = (1 − u − v)v0 + u * v1 + v * v2
            // v0, v1, v2 are the triangle vertices
            // Create 10 < M < 20 endpoints
            // Shoot shadow rays from x -> q_i - q_M
            // Calculate V(x, q_i) and G(x, q_i)
            // Sum (fr * V * G)

            specularBounce = false;

            // Reflect in random direction
            const float phi = PIx2 * invP * rng.getUniform1D();
            // Russian roulette
            if (phi > PIx2) {
                return L;
            }

            const auto nLights = static_cast<int>(lights.size());
            int lightSourceIdx = std::min(static_cast<int>(rng.getUniform1D() * nLights), nLights - 1);

            AreaLight *l = lights[lightSourceIdx].get();
            Shape *tri = l->T.get();
            ColorDbl Ld{0, 0, 0};
            for (unsigned int i = 0; i < Options::nrLightSamples; i++) {
                const glm::vec2 r2 = rng.getUniform2D();
                const glm::vec3 q = tri->getRandomPoint(r2); // random point on the triangle
                const glm::vec3 wiWorld = glm::normalize(q - hitPoint);
                IntersectionInfo shadowIsect;
                const Ray shadowRay(hitPoint + n * epsilon, wiWorld);
                if (intersect(shadowRay, &shadowIsect) && shadowIsect.shape == tri) {
                    // No occlusion! Add contribution from this ray
                    const glm::vec3 wi = glm::normalize(worldToLocal(ss, ts, n, wiWorld));
                    const glm::vec3 shadowN = shadowIsect.n;
                    const ColorDbl f = shadowIsect.brdf->fr(wi, wo);

                    // Calculate geometric term
                    float G = glm::abs((glm::dot(shadowIsect.wo, shadowN) * glm::dot(wiWorld, n))) /
                              glm::distance2(q, hitPoint);
                    Ld += f * G;
                }
            }
            L += static_cast<float>(nLights) / static_cast<float>(Options::nrLightSamples) * l->area * l->L0 * l->intensity * Ld;


            float theta = glm::sqrt(rng.getUniform1D());
            const glm::vec3 wi = glm::normalize(sphericalToCartesian(theta, phi));
            const glm::vec3 wiWorld = glm::normalize(localToWorld(ss, ts, n, wi));

            const Ray newRay(hitPoint + n * epsilon, wiWorld);
            return L + (PI * trace(newRay, rng, depth + 1, specularBounce) * isect.brdf->fr(wi, wo)) * invP;

        } else if (isect.brdf->getType() == BSDF_SPECULAR) {

            specularBounce = true;

            const glm::vec3 R = normalize(reflect(ray.d, n));
            const glm::vec3 bias = epsilon * n;
            const Ray reflectionRay(hitPoint + bias, R);

            return trace(reflectionRay, rng, depth + 1, specularBounce);

        } else if (isect.brdf->getType() == BSDF_TRANSPARENT) {
            // Send reflection and refraction rays
            // Don't really need a brdf here, just need to send the rays and
            // calculate the importance contribution

            specularBounce = true;
            const glm::vec3 I = ray.d;
            const float index = isect.brdf->index;

            const float cosThetaI = glm::dot(I, n);
            glm::vec3 nI = n;
            float eta = index;
            const bool entering = cosThetaI < 0;
            if (entering) {
                eta = 1.0f / index;
            } else {
                nI = -n;
            }

            const glm::vec3 R = glm::normalize(glm::reflect(I, nI));
            glm::vec3 T;

            const glm::vec3 bias = epsilon * n;
            const glm::vec3 reflectionRayOrig = entering ? hitPoint + bias : hitPoint - bias;
            const Ray reflectionRay(reflectionRayOrig, R);

            if (!refract(&T, I, nI, eta)) { // returns false for total internal reflection
                return trace(reflectionRay, rng, depth + 1, specularBounce);
            }

            const float Rs = entering ? fresnel(glm::dot(-I, nI), eta)
                                      : fresnel(glm::dot(T, n), eta);

            // ---------------------
            // Reflection

            const glm::vec3 refractionRayOrig = entering ? hitPoint - bias : hitPoint + bias;
            const Ray refractionRay(refractionRayOrig, T);

            return trace(reflectionRay, rng, depth + 1, specularBounce) * Rs +
                   trace(refractionRay, rng, depth + 1, specularBounce) * (1.0f - Rs);


        }


        return L;
    }


    bool Scene::intersect(const Ray &ray, IntersectionInfo *isect) const {
        // Check for intersection with every object in the scene
        bool hit = false;
        for (auto &&o : objects) {
            if (o.intersect(ray, isect)) {
                hit = true;
            }
        }

        return hit;
    }
}
