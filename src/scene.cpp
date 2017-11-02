//
// Created by Niclas Olmenius on 2017-09-06.
//

#include <glm/gtx/norm.hpp>
#include "camera.h"
#include "scene.h"
#include "ray.h"
#include "intersectioninfo.h"

namespace rays {

    ColorDbl Scene::trace(const Ray &primaryRay, RNG &rng) const {

        ColorDbl L{0.0, 0.0, 0.0};
        ColorDbl throughput{1.0, 1.0, 1.0}; // throughput
        bool specularBounce = false;
        Ray ray(primaryRay.o, primaryRay.d);

        for (unsigned int bounces = 0; bounces < options::maxDepth; bounces++) {
            IntersectionInfo isect;
            bool intersected = intersect(ray, &isect);

            if (!intersected || bounces >= options::maxDepth) {
//                std::cerr << "Ray escaped scene. " << ray << std::endl;
                break;
            }
//            std::cout << throughput << std::endl;

            glm::vec3 P = isect.p;
            glm::vec3 N = isect.n;
            glm::vec3 woWorld = -ray.d;

            // Create local coordinate system
            glm::vec3 ss, ts;
            coordinateSystem(N, &ss, &ts);

            // Transform wo to local
            glm::vec3 woLocal = glm::normalize(worldToLocal(ss, ts, N, woWorld));

            // -----------------------------------------------------------------------
            // Add contribution from emissive surface
            if (bounces == 0 || specularBounce) {
                // TODO Refactor to area light source Le()
                if (auto &&l = isect.obj->getAreaLight()) {
                    // bsdf light pdf
                    // mis weight
                    // *******************************************************************
                    ColorDbl Le = l->L0 * l->intensity; // Lambertian light source
                    // *******************************************************************
                    L += throughput * Le; // * weight / bsdfPdf
                }
            }

            // -----------------------------------------------------------------------
            constexpr float epsilon = 1e-5f;
            if (isect.brdf->getType() == BSDF_DIFFUSE) {

                // Next event estimation (light sampling with shadow rays)

                // Sample light source with MIS
                // TODO Refactor to SampleLight() function
                // *******************************************************************
                const auto nLights = static_cast<int>(lights.size());
                int lightSourceIdx = std::min(static_cast<int>(rng.getUniform1D() * nLights), nLights - 1);

                ColorDbl Ld{0, 0, 0};
                ColorDbl Li{0, 0, 0};
                AreaLight *light = lights[lightSourceIdx].get();
                Shape *tri = light->T.get();
                for (unsigned int j = 0; j < options::nrLightSamples; j++) {
                    const glm::vec2 r2 = rng.getUniform2D();
                    const glm::vec3 q = tri->getRandomPoint(r2); // random point on the triangle
                    const glm::vec3 wiWorld = glm::normalize(q - P);
                    IntersectionInfo shadowIsect;
                    const Ray shadowRay(P + N * epsilon, wiWorld);
                    if (intersect(shadowRay, &shadowIsect) && shadowIsect.shape == tri) {
                        // No occlusion! Add contribution from this ray
                        const glm::vec3 wiLocal = glm::normalize(worldToLocal(ss, ts, N, wiWorld));
                        glm::vec3 shadowN = shadowIsect.n;
                        const ColorDbl f = isect.brdf->sample(wiLocal, woLocal) * absDot(wiWorld, isect.n);
                        float lightBsdfPdf = isect.brdf->pdf(wiLocal, woLocal);
                        // Pdf scaled by geometry term
                        // TODO Refactor geometry term calculation to function G()
                        float lightPdf = light->pdf() * glm::distance2(q, P) / absDot(shadowN, -wiWorld);
                        if (std::isinf(lightPdf)) lightPdf = 0;

                        if (lightPdf > 0) {
                            Li = glm::dot(shadowIsect.n, wiWorld) < 0 ? light->L0 * light->intensity : ColorDbl{0, 0,
                                                                                                                0};
                            float weight = balanceHeuristic(lightPdf, lightBsdfPdf);

                            Ld += f * Li * weight / lightPdf;
                        }
                    }
                }

                Ld /= static_cast<float>(options::nrLightSamples);


                // *******************************************************************
                // Sample BSDF with MIS
                float phi = PIx2 * invP * rng.getUniform1D();
                float theta = glm::sqrt(rng.getUniform1D());
                glm::vec3 wiLocal = glm::normalize(sphericalToCartesian(theta, phi));
                glm::vec3 wiWorld = glm::normalize(localToWorld(ss, ts, N, wiLocal));

                ColorDbl f = isect.brdf->sample(wiLocal, woLocal) * absDot(wiWorld, isect.n);
                float bsdfPdf = isect.brdf->pdf(wiLocal, woLocal);

                if (bsdfPdf > 0) {
                    float tHit;
                    IntersectionInfo isectLight;
                    // Early out intersection test
                    Ray shadowRay(P + N * epsilon, wiWorld);
                    if (tri->intersect(shadowRay, &isectLight, &tHit)) {
                        // Proper intersection test
                        if (this->intersect(shadowRay, &isectLight)) {
                            //Light pdf scaled by geometry term
                            // TODO Refactor geometry term calculation to function G()
                            float lightPdf = light->pdf() * glm::distance2(P, isectLight.p) /
                                             absDot(isectLight.n, -wiWorld);
                            if (std::isinf(lightPdf)) lightPdf = 0;
                            if (lightPdf > 0) {
                                auto &&l = isectLight.obj->getAreaLight();
                                if (l != nullptr) {
                                    Li = glm::dot(isectLight.n, wiWorld) < 0 ? l->L0 * l->intensity : ColorDbl{0, 0, 0};
                                    float weight = balanceHeuristic(bsdfPdf, lightPdf);
                                    Ld += f * Li * weight / bsdfPdf;
                                }
                            }
                        }
                    }
                }


                // Add direct light contribution to path radiance
                L += throughput * static_cast<float>(nLights) * Ld;

                // -----------------------------------------------------------------------
                // Generate a new sample direction from the BSDF
                // TODO Refactor to BSDF::sample()
                // NOTE Only supports diffuse right now

                // *******************************************************************
                // Generate cosine weighted hemispherical coordinates
                phi = PIx2 * invP * rng.getUniform1D();
                // Russian roulette
                if (phi > PIx2) {
                    break;
                }
                theta = glm::sqrt(rng.getUniform1D());

                // Construct the direction vectors
                wiLocal = glm::normalize(sphericalToCartesian(theta, phi));
                wiWorld = glm::normalize(localToWorld(ss, ts, N, wiLocal));
                // *******************************************************************

                // TODO Change fr() to sample() and return wi vector, pdf and f()
                f = isect.brdf->sample(wiLocal, woLocal);
                bsdfPdf = isect.brdf->pdf(wiLocal, woLocal);
                if (bsdfPdf == 0) break;
                throughput *= f * absDot(wiWorld, isect.n) / bsdfPdf * invP;
                specularBounce = isect.brdf->getType() == BSDF_SPECULAR;

                ray = Ray(P + N * epsilon, wiWorld);
            } else if (isect.brdf->getType() == BSDF_TRANSPARENT) {

                // Send reflection and refraction rays
                // Don't really need a brdf here, just need to send the rays and
                // calculate the importance contribution

                specularBounce = true;
                const glm::vec3 I = ray.d;
                const float index = isect.brdf->index;

                const float cosThetaI = glm::dot(I, N);
                glm::vec3 nI = N;
                float eta = index;
                const bool entering = cosThetaI < 0;
                if (entering) {
                    eta = 1.0f / index;
                } else {
                    nI = -N;
                }

                const glm::vec3 R = glm::normalize(glm::reflect(I, nI));
                glm::vec3 T;

                const glm::vec3 bias = epsilon * N;
                const glm::vec3 reflectionRayOrig = entering ? P + bias : P - bias;
                const Ray reflectionRay(reflectionRayOrig, R);

                if (!refract(&T, I, nI, eta)) { // returns false for total internal reflection
                    throughput *= isect.brdf->R;
                    ray = reflectionRay;
                    continue;
                }

                const float Rs = entering ? fresnel(glm::dot(-I, nI), eta)
                                          : fresnel(glm::dot(T, N), eta);

                // Use fresnel coeff to decide if the ray reflects or refracts
                // Keeps the width of the path at 1
                const float u = rng.getUniform1D();
                const glm::vec3 refractionRayOrig = entering ? P - bias : P + bias;
                const Ray refractionRay(refractionRayOrig, T);
                throughput *= isect.brdf->R;
                if (Rs > u) {
                    ray = reflectionRay;
                } else {
                    ray = refractionRay;
                }

            } else if (isect.brdf->getType() == BSDF_SPECULAR) {

                specularBounce = true;
                const glm::vec3 R = normalize(reflect(ray.d, N));
                const glm::vec3 bias = epsilon * N;
                const Ray reflectionRay(P + bias, R);
                throughput *= isect.brdf->R;

                ray = reflectionRay;
            }


        }


        return L;
    }


    bool Scene::intersect(const Ray &ray, IntersectionInfo *isect) const {
        // Check for intersection with every object in the scene
        for (auto &&o : objects) {
            o.intersect(ray, isect);
        }
        return ray.tMax < std::numeric_limits<float>::max();
    }

    bool Scene::intersectShadow(const Ray &ray, IntersectionInfo *isect) const {
        bool hit = false;
        for (auto &&o : objects) {
            if (o.bsdf->getType() != BSDF_TRANSPARENT) {
                if (o.intersect(ray, isect)) { hit = true; }
            }
        }

        return hit;
    }

}
