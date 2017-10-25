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

        if (depth == maxDepth) {
//            std::cout << "Max depth reached. L = " << L << std::endl;
            return L;
        }

        // Check intersection
        IntersectionInfo isect;
        bool intersected = intersect(ray, &isect);

        if (!intersected) {
            std::cerr << "Ray escaped scene" << std::endl;
            return L;
        }

        glm::vec3 woWorld = isect.wo; // Should be normalized
        glm::vec3 n = isect.n; // Should be normalized
        const glm::vec3 hitPoint = isect.p;


        // Create local coordinate system
        glm::vec3 ss, ts;
        coordinateSystem(n, &ss, &ts);

        // Transform wo to local
        glm::vec3 wo = normalize(worldToLocal(ss, ts, n, woWorld));

        // From here on, the calculations are in the local coordinate system of the
        // current hemisphere

        // Intersected object is an area light source.
        // Add contribution from light and terminate.
        // We only care about the first bounce and specular reflections
        if (auto l = isect.obj->getAreaLight()) {
            if (depth == 0) {
                L += l->L0 * l->intensity;
            }
            return L;
//            return l->L0;
        }

        const float epsilon = 1e-5f;

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
            float phi = ((2.f * PI) * invP) * rng.getUniform1D();
            // Russian roulette
            if (phi > 2.f * PI) {
                return L;
            }

            const int M = 5; // TODO move this
            for (const auto &l : lights) {
//                if (isect.obj->getAreaLight() == l.get()) continue; // Lights shouldn't light themselves

                Shape *tri = l->T.get();
                ColorDbl Ld{0, 0, 0};
                for (unsigned int i = 0; i < M; i++) {
                    glm::vec3 q = tri->getRandomPoint(rng); // random point on the triangle
                    glm::vec3 wiWorld = normalize(q - hitPoint);
                    IntersectionInfo shadowIsect;
                    Ray shadowRay(hitPoint + n * epsilon, wiWorld);
                    if (intersect(shadowRay, &shadowIsect) && shadowIsect.shape == tri) {
                        // No occlusion! Add contribution from this ray
                        glm::vec3 wi = worldToLocal(ss, ts, n, wiWorld);
                        wi = normalize(wi);
                        glm::vec3 shadowN = normalize(shadowIsect.n);
                        ColorDbl f = isect.brdf->fr(&wi, wo);

                        // Calculate geometric term
//                        float G = (absDot(shadowIsect.wo, shadowN) * absDot(wiWorld, n)) /
//                                  glm::distance2(q, hitPoint);
                        float G = abs((dot(shadowIsect.wo, shadowN) * dot(wiWorld, n))) /
                                  glm::distance2(q, hitPoint);
                        Ld += f * G;
                    }
                }
                L += (PI * l->area * l->L0 * l->intensity * Ld) / M;
            }


//            float theta = std::acos(std::sqrt(rng.getUniform1D()));
            float r = std::sqrt(rng.getUniform1D());
//            glm::vec3 wi = {std::cos(phi) * std::sin(theta), std::sin(phi) * std::sin(theta), std::cos(theta)};
            glm::vec3 wi = {r * std::sin(phi), r * std::cos(phi), std::sqrt(1 - r * r)};
            wi = normalize(wi);
            glm::vec3 wiWorld = normalize(localToWorld(ss, ts, n, wi));

            Ray newRay(hitPoint + n * epsilon, wiWorld);
            return L + (PI * trace(newRay, rng, depth + 1, specularBounce) * isect.brdf->fr(&wi, wo)) * invP;

        } else if (isect.brdf->getType() == BSDF_SPECULAR) {

            specularBounce = true;

            glm::vec3 R = normalize(reflect(ray.d, n));
            const glm::vec3 bias = epsilon * n;
            Ray reflectionRay(hitPoint + bias, R);

            return trace(reflectionRay, rng, depth + 1, specularBounce);

        } else if (isect.brdf->getType() == BSDF_TRANSPARENT) {
            // Send reflection and refraction rays
            // Don't really need a brdf here, just need to send the rays and
            // calculate the importance contribution

            specularBounce = true;
            glm::vec3 I = ray.d;
            const float index = isect.brdf->index;

            float cosThetaI = dot(I, n);
            glm::vec3 nI = n;
            float eta = index;
            bool entering = cosThetaI < 0;
            if (entering) {
                eta = 1.0f / index;
            } else {
                nI = -n;
            }

            glm::vec3 R = normalize(reflect(I, nI));
            glm::vec3 T;

            const glm::vec3 bias = epsilon * n;
            const glm::vec3 reflectionRayOrig = entering ? hitPoint + bias : hitPoint - bias;
            Ray reflectionRay(reflectionRayOrig, R);

            if (!refract(&T, I, nI, eta)) { // returns false for total internal reflection
                return trace(reflectionRay, rng, depth + 1, specularBounce);
            }

            float Rs;

            if (entering) {
                Rs = fresnel(dot(-I, nI), eta);
            } else {
                Rs = fresnel(dot(T, n), eta);
            }


            // ---------------------
            // Reflection

            glm::vec3 refractionRayOrig = entering ? hitPoint - bias : hitPoint + bias;
            Ray refractionRay(refractionRayOrig, T);

            return trace(reflectionRay, rng, depth + 1, specularBounce) * Rs +
                   trace(refractionRay, rng, depth + 1, specularBounce) * (1 - Rs);


        }


        return L;
    }


    bool Scene::intersect(const Ray &ray, IntersectionInfo *isect) const {
        // Check for intersection with every object in the scene
        bool hit = false;
        for (const auto &o : objects) {
            if (o.intersect(ray, isect)) {
                hit = true;
            }
        }

        return hit;
    }
}
