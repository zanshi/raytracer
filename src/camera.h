//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "common.h"
#include "vertex.h"
#include "pixel.h"


namespace rays {

    using ColorChar = std::array<unsigned char, 3>;

    class Camera {
        using CameraPlane = std::vector<std::vector<Pixel>>;
    public:
        //Camera(std::initializer_list<Vertex> eyeList, unsigned int x, unsigned int y);

        explicit Camera(std::vector<Vertex> eyes = {{-2, 0, 0, 1}}, unsigned int x = 1000, unsigned int y = 1000,
                        unsigned int currEyeIdx = 0) : eyes(std::move(eyes)),
                                                       plane(x, std::vector<Pixel>(y)),
                                                       eyeIdx(currEyeIdx), dx(2.0f / x) {}

        Camera(std::initializer_list<Vertex> e, unsigned int x = 1000, unsigned int y = 1000,
               unsigned int currEyeIdx = 0) : eyes(e),
                                              plane(x, std::vector<Pixel>(y)),
                                              eyeIdx(currEyeIdx), dx(2.0f / x) {}

        void render(const Scene &scene);

        void createImage() const;

    private:

        ColorChar toneMap(ColorDbl c, double iMax) const;

        double getMaxPixelColorVal() const;

        const std::vector<Vertex> eyes;
        CameraPlane plane;

        unsigned int eyeIdx{0};
        const float dx{0.002};

    };

}


#endif //RAYTRACER_CAMERA_H
