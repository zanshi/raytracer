//
// Created by Niclas Olmenius on 2017-09-06.
//

#include "camera.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb/stb_image_write.h>

namespace rays {

    void Camera::render(const Scene &scene) {
        // Loop over each pixel
        // Tile this later

        for (auto i = 0; i < plane.size(); i++) {
            for (auto j = 0; j < plane[0].size(); j++) {
                // Create and send ray
            }
        }

    }

    void Camera::createImage() const {

        std::vector<std::vector<ColorChar>> outImg(
                plane.size(),
                std::vector<ColorChar>(plane[0].size()));

        for (auto i = 0; i < plane.size(); i++) {
            for (auto j = 0; j < plane[0].size(); j++) {
                outImg[i][j] = toneMap(plane[i][j].color, getMaxPixelColorVal());
            }
        }

        if (!stbi_write_png("out.png", outImg.size(), outImg[0].size(), 3, outImg.data(), outImg.size() * 3)) {
            std::cout << "Output written to out.png" << std::endl;
        } else {
            std::cerr << "Failed to write file!" << std::endl;
        }
    }



    ColorChar Camera::toneMap(ColorDbl c, double iMax) const {
        // Basic version
        ColorChar rgb{0,0,0};
        rgb[0] = static_cast<unsigned char>(c.r * (255.99 / iMax));
        rgb[1] = static_cast<unsigned char>(c.g * (255.99 / iMax));
        rgb[2] = static_cast<unsigned char>(c.b * (255.99 / iMax));

        return rgb;
    }

    double Camera::getMaxPixelColorVal() const {
        double maxVal = std::numeric_limits<double>::min();
        for (int i = 0; i < plane.size(); i++) {
            for (int j = 0; i < plane[0].size(); j++) {
                maxVal = std::max(plane[i][j].color.r, maxVal);
                maxVal = std::max(plane[i][j].color.g, maxVal);
                maxVal = std::max(plane[i][j].color.b, maxVal);
            }
        }

        return maxVal;
    }
}
