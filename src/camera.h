//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "common.h"
#include "rng.h"
#include "options.h"
#include "colordbl.h"


namespace rays {

    using ColorChar = std::array<unsigned char, 3>;

    class Camera {
        using CameraPlane = std::vector<std::vector<ColorDbl>>;
    public:

        explicit Camera(std::array<glm::vec3, 2> cameraPositions) : eyePos(cameraPositions[Options::eyeIdx]),
                                                                    plane(Options::width,
                                                                          std::vector<ColorDbl>(Options::width)) {}

        void render(Scene scene);

        void createImage(const std::string &filename) const;
        void createRawImage(const std::string &filename) const;

    private:

        ColorChar toneMap(ColorDbl c, double iMax) const;

        double getMaxPixelColorVal() const;

        const glm::vec3 eyePos;

        CameraPlane plane;

    };

}


#endif //RAYTRACER_CAMERA_H
