//
// Created by Niclas Olmenius on 2017-09-06.
//

#include "camera.h"
#include "scene.h"
#include "ray.h"
#include "intersectioninfo.h"
#include <glm/gtx/norm.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb/stb_image_write.h>

namespace rays {


    void Camera::render(const Scene &scene) {
        // Loop over each pixel
        // Tile this later
        const float offset = 1.0f - 1.0f / plane.size();

		CameraPlane tempPlane = plane;

        RNG rng{};
#pragma omp parallel for default(none) shared(tempPlane, scene) private(rng) schedule(dynamic, 16)
        for (int i = 0; i < plane.size(); i++) {
            for (int j = 0; j < plane[0].size(); j++) {
                ColorDbl L{0, 0, 0};
                Vector3f direction;

                for (unsigned int k = 0; k < nSamples; k++) {

                    // Determine position on camera plane
                    direction = normalize(Vertex3f{0, i * dx - offset, j * dx - offset} - eyes[eyeIdx]);

                    // Create eye -> camera plane ray
                    auto ray = Ray(eyes[eyeIdx], direction);
                    L += trace(ray, scene, rng, 0, false);
                }

                tempPlane[i][j].color = L / nSamples;
            }
        }

		plane = tempPlane;
    }

    void Camera::createImage(const std::string &filename) {

        auto height = plane.size();
        auto width = plane[0].size();

        std::vector<unsigned char> outImg(width * height * 3);

        double maxVal = getMaxPixelColorVal();
        std::cout << "Highest pixel value = " << maxVal << std::endl;

        for (unsigned int i = 0; i < plane.size(); i++) {
            for (unsigned int j = 0; j < plane[0].size(); j++) {
                auto ldr = toneMap(plane[i][j].color, maxVal);
                outImg[3 * (width * j + i) + 0] = ldr[0];
                outImg[3 * (width * j + i) + 1] = ldr[1];
                outImg[3 * (width * j + i) + 2] = ldr[2];
            }
        }

        if (stbi_write_png(filename.data(), static_cast<int>(width), static_cast<int>(height), 3, outImg.data(),
                           static_cast<int>(width * 3))) {
            std::cout << "Output written to out.png" << std::endl;
        } else {
            std::cerr << "Failed to write file!" << std::endl;
        }
    }

    ColorChar Camera::toneMap(ColorDbl c, double iMax) const {
        ColorChar rgb{0, 0, 0};
        c = clamp(c, 0.0, 1.0);
        rgb[0] = static_cast<unsigned char>(c.r * 255);
        rgb[1] = static_cast<unsigned char>(c.g * 255);
        rgb[2] = static_cast<unsigned char>(c.b * 255);

        return rgb;
    }

    double Camera::getMaxPixelColorVal() const {
        double maxVal = std::numeric_limits<double>::min();
        for (const auto &i : plane) {
            for (unsigned int j = 0; j < plane[0].size(); j++) {
                maxVal = i[j].color.maxColorVal(maxVal);
            }
        }

        return maxVal;
    }

    ColorDbl Camera::trace(Ray &ray, const Scene &scene, RNG &rng, int depth, bool specularBounce) const {

        ColorDbl L{0.0, 0.0, 0.0};

        if (depth == maxDepth) {
//            std::cout << "Max depth reached. L = " << L << std::endl;
            return L;
        }

        // Check intersection
        IntersectionInfo isect;
        bool intersected = scene.intersect(ray, &isect);

        if (!intersected) {
//            std::cerr << "Ray escaped scene" << std::endl;
            return L;
        }

        Vector3f woWorld = isect.wo; // Should be normalized
        Vector3f n = isect.n; // Should be normalized

        // Create local coordinate system
        Vector3f ss, ts;
        coordinateSystem(n, &ss, &ts);

        // Transform wo to local
        Vector3f wo = normalize(worldToLocal(ss, ts, n, woWorld));

        // From here on, the calculations are in the local coordinate system of the
        // current hemisphere

        // Intersected object is an area light source.
        // Add contribution from light and terminate.
        // We only care about the first bounce and specular reflections
        if (auto l = isect.obj->getAreaLight()) {
            if (depth == 0 || specularBounce) {
                L += l->L0 * l->intensity;
            }
            return L;
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


            const int M = 10; // TODO move this
            for (const auto &l : scene.lights) {
//                if (isect.obj->getAreaLight() == l.get()) continue; // Lights shouldn't light themselves
                ColorDbl Ld{0, 0, 0};
                for (unsigned int i = 0; i < M; i++) {
                    Vertex3f q = l->T->getRandomPoint(rng); // random point on the triangle
                    Vector3f wiWorld = normalize(q - isect.p);
                    IntersectionInfo shadowIsect;
                    Ray shadowRay(isect.p + n * epsilon, wiWorld);
                    if (scene.intersect(shadowRay, &shadowIsect) && shadowIsect.shape == l->T.get()) {
                        // No occlusion! Add contribution from this ray
                        Vector3f wi = worldToLocal(ss, ts, n, wiWorld);
                        wi = normalize(wi);
                        Vector3f shadowN = normalize(shadowIsect.n);
                        ColorDbl f = isect.brdf->fr(&wi, wo);

                        // Calculate geometric term
                        // TODO Check if this is valid for spheres (probably not)
                        float G = (absDot(shadowIsect.wo, shadowN) * absDot(wiWorld, n)) /
                                  distance2(q, isect.p);
                        Ld += f * G;
                    }
                }
                L += (l->area * l->L0 * l->intensity * Ld) / M;
            }

            // Reflect in random direction
            float phi = ((2.f * PI) / P) * rng.getUniform1D();
            // Russian roulette
            if (phi > 2.f * PI) {
                return L;
            }

            float theta = std::acos(std::sqrt(rng.getUniform1D()));
            Vector3f wi = {std::cos(phi) * std::sin(theta), std::sin(phi) * std::sin(theta), std::cos(theta)};
            wi = normalize(wi);
            Vector3f wiWorld = normalize(localToWorld(ss, ts, n, wi));

            Ray newRay(isect.p + n * epsilon, wiWorld);
            L += (PI * trace(newRay, scene, rng, depth + 1, specularBounce) * isect.brdf->fr(&wi, wo)) / P;

        } else if (isect.brdf->getType() == BSDF_TRANSPARENT) {
            // Send reflection and refraction rays
            // Don't really need a brdf here, just need to send the rays and
            // calculate the importance contribution

            specularBounce = true;
            Vector3f I = ray.d;
            Vector3f RWorld = normalize(reflect(I, n));
            Vector3f T;

            // ---------------------
            // Reflection
            float fresnelReflectionCoefficient = fresnel(dot(I,n), isect.brdf->index);

            bool outside = dot(I,n) < 0;
            Vector3f bias = epsilon * n;

            Vertex3f reflectionRayOrig = outside ? isect.p + bias : isect.p - bias;
            Ray reflectionRay(reflectionRayOrig, RWorld);
            ColorDbl traced = trace(reflectionRay, scene, rng, depth + 1, specularBounce);
            ColorDbl rColor = fresnelReflectionCoefficient * isect.brdf->R * traced;
            L += rColor;

            // ---------------------
            // Refraction
            if (refract(&T, I, n, isect.brdf->index)) {
                Vector3f TWorld = normalize(T);
                Vertex3f refractionRayOrig = outside ? isect.p - bias : isect.p + bias;
                Ray refractedRay(refractionRayOrig, TWorld);
                ColorDbl TColor = trace(refractedRay, scene, rng, depth + 1, specularBounce);
                L += (1.0f - fresnelReflectionCoefficient) * isect.brdf->R * TColor;
            }
        }


        return L;
    }


}
