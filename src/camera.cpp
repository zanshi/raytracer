//
// Created by Niclas Olmenius on 2017-09-06.
//

#include "camera.h"
#include "scene.h"
#include "ray.h"
#include "vertex.h"
#include "vector.h"
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
                // TODO Refactor this to a function
                auto pointOnPlane = Vertex{0, i * dx - offset, j * dx - offset, 1.0};
                // Create ray
                auto ray = Ray(eyes[eyeIdx], pointOnPlane);

                if (scene.intersect(ray)) {
                    auto &&pixel = plane[i][j];
                    pixel.color = ray.color; // For now set pixel color to ray color
                    pixel.rays.emplace_back(std::make_shared<Ray>(ray)); // Save ray in pixel
                    // Ray hit a shape in the scene, save for later? probably
                }
            }
        }

    }

    void Camera::createImage() const {

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

        if (stbi_write_png("out.png", width, height, 3, outImg.data(), width * 3)) {
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
}
