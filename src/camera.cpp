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
        constexpr float offset = 1.0f - 1.0f / options::width;
        constexpr unsigned width = options::width;
        constexpr unsigned height = width;

        RNG rng{};
#pragma omp parallel for collapse(2) default(none) shared(plane, scene) private(rng) schedule(dynamic, 16)
        for (unsigned int y = 0; y < width; y++) {
            for (unsigned int z = 0; z < height; z++) {
                ColorDbl L{0, 0, 0};

                plane[y * options::width + z] = L;

                for (unsigned int sy = 0; sy < 2; sy++) {
                    for (unsigned int sz = 0; sz < 2; sz++, L = {0, 0, 0}) {
                        for (unsigned int k = 0; k < options::nrSamples; k++) {

                            // Determine position on camera plane
                            const float r1 = 2 * rng.getUniform1D();
                            const float r2 = 2 * rng.getUniform1D();
                            float offY = r1 < 1 ? glm::sqrt(r1) - 1 : 1 - glm::sqrt(2 - r1);
                            float offZ = r2 < 1 ? glm::sqrt(r2) - 1 : 1 - glm::sqrt(2 - r2);

                            offY = (sy + 0.5f + offY) / 2.0f + y;
                            offZ = (sz + 0.5f + offZ) / 2.0f + z;

//                            direction = glm::normalize(glm::vec3{0, y * dx - offset, offset - z * dx} - eyes[eyeIdx]);
                            const auto direction = glm::normalize(
                                    glm::vec3{0, offY * options::dx - offset, offset - offZ * options::dx} - eyePos);

                            // Create eye -> camera plane ray
                            const Ray ray(eyePos, direction);

                            L += scene.trace(ray, rng);
                        }
                        L = L / options::nrSamples;

                        plane[y * options::width + z] += L * 0.25;
                    }
                }

            }
        }
    }

    void Camera::createImage(const std::string &filename) const {

        auto height = options::width;
        auto width = height;

        std::string name = filename + ".png";

        std::vector<unsigned char> outImg(width * height * 3);

        double maxVal = getMaxPixelColorVal();
        std::cout << "Highest pixel value = " << maxVal << std::endl;

        for (unsigned int i = 0; i < height; i++) {
            for (unsigned int j = 0; j < width; j++) {
                auto ldr = toneMap(plane[i * width + j], maxVal);
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
        for (auto &&i : plane) {
            maxVal = i.maxColorVal(maxVal);
        }
        return maxVal;
    }

    void Camera::createRawImage(const std::string &filename) const {
        auto height = options::width;
        auto width = height;

        std::string name = filename + ".hdr";

        std::vector<float> outImg(width * height * 3);

        for (unsigned int i = 0; i < width; i++) {
            for (unsigned int j = 0; j < height; j++) {
//                ColorDbl clamped = clamp(plane[i][j], 0.0, 1.0);
                ColorDbl clamped = plane[i * width + j];
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
