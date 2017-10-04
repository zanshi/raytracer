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

#include <omp.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb/stb_image_write.h>

namespace rays {

    void Camera::render(const Scene &scene) {
        // Loop over each pixel
        // Tile this later
        float offset = 1.0f - 1.0f / plane.size();


#pragma omp parallel for
        for (auto i = 0; i < plane.size(); i++) {
            for (auto j = 0; j < plane[0].size(); j++) {
                // Determine position on camera plane
                auto direction = Vertex3f{0, i * dx - offset, j * dx - offset} - eyes[eyeIdx];
                direction = normalize(direction);

                // Create eye -> camera plane ray
                auto ray = Ray(eyes[eyeIdx], direction);

                RNG rng;
                ColorDbl beta{1, 1, 1};
                ColorDbl L = trace(ray, scene, rng, 0, beta);

                auto &&pixel = plane[i][j];
                pixel.color = L; // For now set pixel color to ray color
                pixel.rays.emplace_back(std::make_shared<Ray>(ray)); // Save ray in pixel
                // Ray hit a shape in the scene, save for later? probably
            }
        }

    }

    void Camera::createImage(const std::string &filename) const {

        auto height = plane.size();
        auto width = plane[0].size();

        std::vector<unsigned char> outImg(width * height * 3);

        double maxVal = getMaxPixelColorVal();

        for (auto i = 0; i < plane.size(); i++) {
            for (auto j = 0; j < plane[0].size(); j++) {
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


    ColorChar Camera::toneMap(ColorDbl c, double iMax) const {
        // Basic version
        ColorChar rgb{0, 0, 0};
        rgb[0] = static_cast<unsigned char>(c.r * (255.99 / iMax));
        rgb[1] = static_cast<unsigned char>(c.g * (255.99 / iMax));
        rgb[2] = static_cast<unsigned char>(c.b * (255.99 / iMax));

        return rgb;
    }

    double Camera::getMaxPixelColorVal() const {
        double maxVal = std::numeric_limits<double>::min();
        for (int i = 0; i < plane.size(); i++) {
            for (int j = 0; j < plane[0].size(); j++) {
                maxVal = std::max(plane[i][j].color.r, maxVal);
                maxVal = std::max(plane[i][j].color.g, maxVal);
                maxVal = std::max(plane[i][j].color.b, maxVal);
            }
        }

        return maxVal;
    }

    ColorDbl Camera::trace(Ray &ray, const Scene &scene, RNG &rng, int depth, ColorDbl &beta) const {

        if (depth == maxDepth) { return {0, 0, 0}; }

        // TODO: Check russian roulette

        ColorDbl L{0, 0, 0};

        // 1. Check intersection
        IntersectionInfo isect;
        bool intersected = scene.intersect(ray, &isect);

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
        Vector3f n = normalize(isect.n);
        Vector3f wi;

        // Local coordinate system base vectors
        Vector3f ss = woWorld - (woWorld.dot(n)) * n;
        Vector3f ts = n.cross(ss);

        // Transform wo to local
        Vector3f wo = worldToLocal(ss, ts, n, woWorld);

        // From here on, the calculations are in the local coordinate system of the
        // current hemisphere

        switch (isect.brdf->getType()) {
            // Whitted: Just add a diffuse light term
            // Monte-Carlo: Use pdf and rng to calculate the reflected ray
            case BSDF_DIFFUSE: {

                // Add direct light contribution



                // Reflect in random direction
                float phi = 2 * M_PI * rng.getUniform1D();
                float theta = std::acos(std::sqrt(rng.getUniform1D()));
                wi = {std::cos(phi) * std::sin(theta), std::sin(phi) * std::sin(theta), std::cos(theta)};

                ColorDbl f = isect.brdf->fr(&wi, wo);


                // TODO: Implement visibility check with shadow rays



            }
                break;

            case BSDF_SPECULAR: {// Send reflection ray
                // L+= specularReflection

            }
                break;
            case BSDF_TRANSPARENT: {// Send reflection and refraction rays
                // Don't really need a brdf here, just need to send the rays and
                // calculate the importance contribution

                Vector3f R = reflect(wo, n);
                Vector3f T;

                // TODO: beta instead of L

                // ---------------------
                // Reflection
                float fresnelReflectionCoefficient = fresnel(n.dot(wo), isect.brdf->index);
                Ray reflected(isect.p, localToWorld(ss, ts, n, R));
                L += fresnelReflectionCoefficient * trace(reflected, scene, rng, depth + 1, beta) * absDot(R, n);

                // ---------------------
                // Refraction
                if (refract(&T, wo, n, isect.brdf->index)) {
                    Ray refracted(isect.p, localToWorld(ss, ts, n, T));
                    L += (1 - fresnelReflectionCoefficient) * trace(refracted, scene, rng, depth + 1, beta) *
                         absDot(T, n);
                }

            }
                break;
        }


        // 2.
        // Diffuse:
        //      Whitted: Terminate, calculate local lighting contribution
        //      Monte-Carlo: Calculate random reflection ray (unless Russian roulette terminates it)
        // Specular (mirror): Calculate reflection ray (as in Whitted)
        // Transparent: Calculate reflection and refraction rays (as in Whitted)
        // Area Light: Terminate


        // 3. Calculate direct light contribution



        return L;
    }

}
