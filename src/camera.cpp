//
// Created by Niclas Olmenius on 2017-09-06.
//

#include "camera.h"
#include "scene.h"
#include "ray.h"
#include <glm/gtx/norm.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb/stb_image_write.h>

namespace rays {


    void Camera::render(Scene scene) {
        // Loop over each pixel
        // Tile this later
        const float offset = 1.0f - 1.0f / plane.size();
        const int width = Options::width;
        const int height = width;

        CameraPlane tempPlane = plane;

        RNG rng{};
#pragma omp parallel for collapse(2) default(none) shared(tempPlane) firstprivate(scene) private(rng) schedule(dynamic, 16)
        for (int y = 0; y < width; y++) {
            for (int z = 0; z < height; z++) {
                ColorDbl L = {0, 0, 0};

                for (int sy = 0; sy < 2; sy++) {
                    for (int sz = 0; sz < 2; sz++, L = {0, 0, 0}) {
                        for (unsigned int k = 0; k < Options::nrSamples; k++) {

                            // Determine position on camera plane
                            float r1 = 2 * rng.getUniform1D();
                            float r2 = 2 * rng.getUniform1D();
                            float offY = r1 < 1 ? glm::sqrt(r1) - 1 : 1 - glm::sqrt(2 - r1);
                            float offZ = r2 < 1 ? glm::sqrt(r2) - 1 : 1 - glm::sqrt(2 - r2);

                            offY = (sy + 0.5f + offY) / 2 + y;
                            offZ = (sz + 0.5f + offZ) / 2 + z;

//                            direction = glm::normalize(glm::vec3{0, y * dx - offset, offset - z * dx} - eyes[eyeIdx]);
                            auto direction = glm::normalize(
                                    glm::vec3{0, offY * Options::dx - offset, offset - offZ * Options::dx} - eyePos);

                            // Create eye -> camera plane ray
                            const Ray ray(eyePos, direction);

                            L += scene.trace(ray, rng, 0, false);
                        }
                        L = L / Options::nrSamples;

                        tempPlane[y][z] += L * 0.25;
                    }
                }

            }
        }

        plane = tempPlane;
    }

    void Camera::createImage(const std::string &filename) const {

        auto height = plane.size();
        auto width = plane[0].size();

        std::string name = filename + ".png";

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

        if (stbi_write_png(name.data(), static_cast<int>(width), static_cast<int>(height), 3, outImg.data(),
                           static_cast<int>(width * 3))) {
            std::cout << "Output written to " << name << std::endl;
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

    void Camera::createRawImage(const std::string &filename) const {
        auto height = plane.size();
        auto width = plane[0].size();

        std::string name = filename + ".hdr";

        std::vector<float> outImg(width * height * 3);

        for (unsigned int i = 0; i < plane.size(); i++) {
            for (unsigned int j = 0; j < plane[0].size(); j++) {
                ColorDbl clamped = clamp(plane[i][j], 0.0, 1.0);
                outImg[3 * (width * j + i) + 0] = static_cast<float>(clamped.r);
                outImg[3 * (width * j + i) + 1] = static_cast<float>(clamped.g);
                outImg[3 * (width * j + i) + 2] = static_cast<float>(clamped.b);
            }
        }

        if (stbi_write_hdr(name.data(), static_cast<int>(width), static_cast<int>(height), 3, outImg.data())) {
            std::cout << "Output written to " << name << std::endl;
        } else {
            std::cerr << "Failed to write file!" << std::endl;

        }


    }
}
