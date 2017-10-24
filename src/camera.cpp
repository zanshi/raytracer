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
		const int width = plane.size();
		const int height = plane[0].size();

		CameraPlane tempPlane = plane;

        RNG rng{};
        glm::vec3 direction;
#pragma omp parallel for collapse(2) default(none) shared(tempPlane, scene) private(rng, direction) schedule(dynamic, 16)
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
				ColorDbl L = { 0,0,0 };

                for (unsigned int k = 0; k < nSamples; k++) {

                    // Determine position on camera plane
                    direction = normalize(glm::vec3{0, i * dx - offset, offset - j * dx} - eyes[eyeIdx]);

                    // Create eye -> camera plane ray
                    const Ray ray(eyes[eyeIdx], direction);

                    L += trace(ray, scene, rng, 0, false);
                }

                tempPlane[i][j] = L / nSamples;
            }
        }

		plane = tempPlane;
    }

    void Camera::createImage(const std::string &filename) const {

        auto height = plane.size();
        auto width = plane[0].size();

        std::vector<unsigned char> outImg(width * height * 3);

        double maxVal = getMaxPixelColorVal();
        std::cout << "Highest pixel value = " << maxVal << std::endl;

        for (unsigned int i = 0; i < plane.size(); i++) {
            for (unsigned int j = 0; j < plane[0].size(); j++) {
                auto ldr = toneMap(plane[i][j], maxVal);
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
		c = gammaCorrect(c);
        rgb[0] = static_cast<unsigned char>(c.r * 255);
        rgb[1] = static_cast<unsigned char>(c.g * 255);
        rgb[2] = static_cast<unsigned char>(c.b * 255);

        return rgb;
    }

    double Camera::getMaxPixelColorVal() const {
        double maxVal = std::numeric_limits<double>::min();
        for (const auto &i : plane) {
            for (unsigned int j = 0; j < plane[0].size(); j++) {
                maxVal = i[j].maxColorVal(maxVal);
            }
        }

        return maxVal;
    }

    ColorDbl Camera::trace(const Ray &ray, const Scene &scene, RNG &rng, int depth, bool specularBounce) const {

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

				Shape* tri = l->T.get();
                ColorDbl Ld{0, 0, 0};
                for (unsigned int i = 0; i < M; i++) {
                    glm::vec3 q = tri->getRandomPoint(rng); // random point on the triangle
                    glm::vec3 wiWorld = normalize(q - hitPoint);
                    IntersectionInfo shadowIsect;
                    Ray shadowRay(hitPoint + n * epsilon, wiWorld);
                    if (scene.intersect(shadowRay, &shadowIsect) && shadowIsect.shape == tri) {
                        // No occlusion! Add contribution from this ray
                        glm::vec3 wi = worldToLocal(ss, ts, n, wiWorld);
                        wi = normalize(wi);
                        glm::vec3 shadowN = normalize(shadowIsect.n);
                        ColorDbl f = isect.brdf->fr(&wi, wo);

                        // Calculate geometric term
                        // TODO Check if this is valid for spheres (probably not)
                        float G = (absDot(shadowIsect.wo, shadowN) * absDot(wiWorld, n)) /
                                  glm::distance2(q, hitPoint);
                        Ld += PI * f * G;
                    }
                }
                L += (l->area * l->L0 * l->intensity * Ld) / M;
            }

            // Reflect in random direction
            float phi = ((2.f * PI) * invP) * rng.getUniform1D();
            // Russian roulette
            if (phi > 2.f * PI) {
				return ColorDbl{};
            }

            float theta = std::acos(std::sqrt(rng.getUniform1D()));
            glm::vec3 wi = {std::cos(phi) * std::sin(theta), std::sin(phi) * std::sin(theta), std::cos(theta)};
            wi = normalize(wi);
            glm::vec3 wiWorld = normalize(localToWorld(ss, ts, n, wi));

            Ray newRay(hitPoint + n * epsilon, wiWorld);
            L += (PI * trace(newRay, scene, rng, depth + 1, specularBounce) * isect.brdf->fr(&wi, wo)) * invP;

        } else if (isect.brdf->getType() == BSDF_TRANSPARENT) {
            // Send reflection and refraction rays
            // Don't really need a brdf here, just need to send the rays and
            // calculate the importance contribution

            specularBounce = true;
            glm::vec3 I = ray.d;
            glm::vec3 RWorld = normalize(reflect(I, n));
            glm::vec3 T;
			const float index = isect.brdf->index;
			auto && brdfColor = isect.brdf->R;

            // ---------------------
            // Reflection
            float fresnelReflectionCoefficient = fresnel(dot(I,n), index);

            bool outside = dot(I,n) < 0;
            glm::vec3 bias = epsilon * n;

            glm::vec3 reflectionRayOrig = outside ? hitPoint + bias : hitPoint - bias;
            Ray reflectionRay(reflectionRayOrig, RWorld);
            ColorDbl traced = trace(reflectionRay, scene, rng, depth + 1, specularBounce);
            ColorDbl rColor = fresnelReflectionCoefficient * brdfColor * traced;
            L += rColor;

            // ---------------------
            // Refraction
            if (refract(&T, I, n, index)) {
                glm::vec3 TWorld = normalize(T);
                glm::vec3 refractionRayOrig = outside ? hitPoint - bias : hitPoint + bias;
                Ray refractedRay(refractionRayOrig, TWorld);
                ColorDbl TColor = trace(refractedRay, scene, rng, depth + 1, specularBounce);
                L += (1.0f - fresnelReflectionCoefficient) * brdfColor * TColor;
            }
        }


        return L;
    }


}
