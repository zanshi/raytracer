//
// Created by Niclas Olmenius on 2017-09-06.
//

#include "camera.h"
#include "scene.h"
#include "ray.h"
#include "vertex.h"
#include "vector.h"
#include "intersectioninfo.h"
#include "rng.h"
#include "bsdf.h"
#include "area_light.h"
#include "colordbl.h"

#include <omp.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb/stb_image_write.h>

namespace rays {


    void Camera::render(const Scene &scene) {
        // Loop over each pixel
        // Tile this later
        const float offset = 1.0f - 1.0f / plane.size();

        RNG rng{};
#pragma omp parallel for default(none) shared(plane, scene) private(rng) schedule(dynamic, 16)
        for (unsigned int i = 0; i < plane.size(); i++) {
            for (unsigned int j = 0; j < plane[0].size(); j++) {
                ColorDbl L{0, 0, 0};
                Vector3f direction;

                for (unsigned int k = 0; k < nSamples; k++) {

                    // Determine position on camera plane
                    direction = normalize(Vertex3f{0, i * dx - offset, j * dx - offset} - eyes[eyeIdx]);

                    // Create eye -> camera plane ray
                    auto ray = Ray(eyes[eyeIdx], direction);


                    ColorDbl beta{1, 1, 1};
                    L += trace(ray, scene, rng, 0, beta);
                }

                plane[i][j].color = L / nSamples;
            }
        }
    }

    void Camera::createImage(const std::string &filename) {

        auto height = plane.size();
        auto width = plane[0].size();

        std::vector<unsigned char> outImg(width * height * 3);

//        adjustLevels();
        double maxVal = getMaxPixelColorVal();
        std::cout << maxVal << std::endl;

        for (unsigned int i = 0; i < plane.size(); i++) {
            for (unsigned int j = 0; j < plane[0].size(); j++) {
                auto ldr = toneMap(plane[i][j].color, maxVal);
//                std::cout << static_cast<int>(ldr[0]) << ' '
//                          << static_cast<int>(ldr[1]) << ' '
//                          << static_cast<int>(ldr[2]) << std::endl;
                outImg[3 * (width * j + i) + 0] = ldr[0];
                outImg[3 * (width * j + i) + 1] = ldr[1];
                outImg[3 * (width * j + i) + 2] = ldr[2];
            }
        }

        if (stbi_write_png(filename.data(), width, height, 3, outImg.data(), width * 3)) {
            std::cout << "Output written to out.png" << std::endl;
        } else {
            std::cerr << "Failed to write file!" << std::endl;
        }
    }

    void Camera::adjustLevels() {
        for (unsigned int i = 0; i < plane.size(); i++) {
            for (unsigned int j = 0; j < plane[0].size(); j++) {
                plane[i][j].color.r = std::sqrt(plane[i][j].color.r);
                plane[i][j].color.g = std::sqrt(plane[i][j].color.g);
                plane[i][j].color.b = std::sqrt(plane[i][j].color.b);
            }
        }
    }

    ColorChar Camera::toneMap(ColorDbl c, double iMax) const {
        // Basic version
        ColorChar rgb = {0, 0, 0};
//        c = gammaCorrect(c);
        c = clamp(c, 0.0, 1.0);
//        rgb[0] = static_cast<unsigned char>(clamp(c.r * (255.99 / iMax), 0.0, 255.0));
//        rgb[1] = static_cast<unsigned char>(clamp(c.g * (255.99 / iMax), 0.0, 255.0));
//        rgb[2] = static_cast<unsigned char>(clamp(c.b * (255.99 / iMax), 0.0, 255.0));
        rgb[0] = static_cast<unsigned char>(c.r * 255);
        rgb[1] = static_cast<unsigned char>(c.g * 255);
        rgb[2] = static_cast<unsigned char>(c.b * 255);

        return rgb;
    }

    double Camera::getMaxPixelColorVal() const {
        double maxVal = std::numeric_limits<double>::min();
        for (unsigned int i = 0; i < plane.size(); i++) {
            for (unsigned int j = 0; j < plane[0].size(); j++) {
                maxVal = std::max(plane[i][j].color.r, maxVal);
                maxVal = std::max(plane[i][j].color.g, maxVal);
                maxVal = std::max(plane[i][j].color.b, maxVal);
            }
        }

        return maxVal;
    }

    ColorDbl Camera::trace(Ray &ray, const Scene &scene, RNG &rng, int depth, ColorDbl &beta) const {

        ColorDbl L{0.0, 0.0, 0.0};

        if (depth == maxDepth) {
//            std::cout << "Max depth reached. L = " << L << std::endl;
            return L;
        }

        // 1. Check intersection
        IntersectionInfo isect;
        bool intersected = scene.intersect(ray, &isect);

        if (!intersected) {
//            std::cerr << "Ray escaped scene" << std::endl;
            return L;
        }

        // Check the intersection surface
        // Different behaviour depending on surface type
        // Walls: Lambertian
        // Objects:
        // 1 Lambertian
        // 1 Oren-Nayar
        // 1 Transparent (Sphere)

        // Reflected ray direction
        // wi = wo - 2 * (wo * N) * N
        Vector3f woWorld = isect.wo;

//        if (woWorld.x == 0 && woWorld.y == 0 && woWorld.z == 0) {
//            std::cout << woWorld << std::endl;
//            std::cout << isect.p << std::endl;
//        }

        Vector3f n = normalize(isect.n);

        // Local coordinate system base vectors
//        Vector3f ss = woWorld - (woWorld.dot(n)) * n;

        Vector3f ss, ts;
        coordinateSystem(n, &ss, &ts);

//        Vector3f ss = normalize(woWorld - (woWorld.dot(n)) * n);
////        Vector3f ss = normalize(Vector3f(-n.y, n.x, 0));
//        Vector3f ts = normalize(ss.cross(n));
//        Vector3f ts = ss.cross(n);

        // Transform wo to local
        Vector3f wo = worldToLocal(ss, ts, n, woWorld);
        wo = normalize(wo);

        // From here on, the calculations are in the local coordinate system of the
        // current hemisphere

        // Intersected object is a light source. Add contribution from light and terminate.
        if (isect.obj->getAreaLight() != nullptr) {
//            if (depth == 0) {
            L += isect.obj->getAreaLight()->L0;
//            }
            return L;
        }

        const float epsilon = 1e-5f;

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

            const int M = 10; // TODO move this

            for (const auto &l : scene.lights) {
                if (isect.obj->getAreaLight() == l.get()) continue; // Lights shouldn't light themselves
                ColorDbl Ld{0, 0, 0};
                float pdf = 1.f / l->area;
                for (unsigned int i = 0; i < M; i++) {
                    Vertex3f q = l->T->getRandomPoint(rng); // random point on the triangle
                    Vector3f wiWorld = normalize(q - isect.p);
                    IntersectionInfo shadowIsect;
                    Ray shadowRay(isect.p + n * epsilon, wiWorld);
                    float tHit = std::numeric_limits<float>::max();
                    if (l->T->intersect(shadowRay, &shadowIsect, &tHit)) {

                        if (shadowIsect.shape == l->T.get()) {
                            // No occlusion! Add contribution from this ray
                            Vector3f wi = worldToLocal(ss, ts, n, wiWorld);
                            wi = normalize(wi);

                            Vector3f shadowN = normalize(shadowIsect.n);
//                            Vector3f shadowSs = shadowIsect.wo - (shadowIsect.wo.dot(shadowN)) * shadowN;
//                            Vector3f shadowTs = shadowSs.cross(shadowN);

//                            Vector3f shadowWoLocal = worldToLocal(shadowSs, shadowTs, shadowN, shadowIsect.wo);
//                            shadowWoLocal = normalize(shadowWoLocal);
                            ColorDbl f = isect.brdf->fr(&wi, wo);
                            // Calculate geometric term (not 100% sure about this one)
                            float G = (absDot(shadowIsect.wo, shadowN) * absDot(wiWorld, n)) /
                                      distanceSquared(q, isect.p);
                            Ld += f * G;
                        }
                    }
                }
                L += (l->area * l->L0 * Ld) / M;
            }

            // Reflect in random direction
            float r1 = rng.getUniform1D();
            float phi = ((2.f * M_PI) / P) * r1;
            // Russian roulette
//            std::cout << r1 << std::endl;
            if (phi > 2.f * M_PI) {
                return L;
            }

            float theta = std::acos(std::sqrt(rng.getUniform1D()));
            Vector3f wi = {std::cos(phi) * std::sin(theta), std::sin(phi) * std::sin(theta), std::cos(theta)};
            wi = normalize(wi);
            Vector3f wiWorld = normalize(localToWorld(ss, ts, n, wi));

            Ray newRay(isect.p + n * epsilon, wiWorld);
//            ColorDbl reflection = trace(newRay, scene, rng, depth + 1, beta);

//            L += (M_PI * reflection * isect.brdf->fr(&wi, wo) * absDot(wiWorld, n)) / (isect.brdf->pdf(wiWorld, n) * P);
            L += (M_PI * trace(newRay, scene, rng, depth + 1, beta) * isect.brdf->fr(&wi, wo)) / P;

        } else if (isect.brdf->getType() == BSDF_TRANSPARENT) {
            // Send reflection and refraction rays
            // Don't really need a brdf here, just need to send the rays and
            // calculate the importance contribution

            Vector3f I = ray.d;
            Vector3f RWorld = normalize(reflect(I, n));
            Vector3f T;

            // ---------------------
            // Reflection
            float fresnelReflectionCoefficient = fresnel(I.dot(n), isect.brdf->index);

            bool outside = I.dot(n) < 0;
            Vector3f bias = epsilon * n;

            Vertex3f reflectionRayOrig = outside ? isect.p + bias : isect.p - bias;
            Ray reflected(reflectionRayOrig, RWorld);
            ColorDbl traced = trace(reflected, scene, rng, depth + 1, beta);
            ColorDbl rColor = fresnelReflectionCoefficient * isect.brdf->R * traced;
            L += rColor;

            // ---------------------
            // Refraction
            if (refract(&T, I, n, isect.brdf->index)) {
                Vector3f TWorld = normalize(T);
                Vertex3f refractionRayOrig = outside ? isect.p - bias : isect.p + bias;
                Ray refracted(refractionRayOrig, TWorld);
                ColorDbl TColor = trace(refracted, scene, rng, depth + 1, beta);
                L += (1.0f - fresnelReflectionCoefficient) * isect.brdf->R * TColor;
            }
        }


        return L;
    }


}
