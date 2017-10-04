//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "common.h"
#include "vertex.h"
#include "pixel.h"
#include "rng.h"


namespace rays {

    using ColorChar = std::array<unsigned char, 3>;

    class Camera {
        using CameraPlane = std::vector<std::vector<Pixel>>;
    public:

        explicit Camera(std::vector<Vertex3f> eyes = {{-2, 0, 0}}, unsigned int x = 1000, unsigned int y = 1000,
                        unsigned int currEyeIdx = 0) : eyes(std::move(eyes)),
                                                       plane(x, std::vector<Pixel>(y)),
                                                       eyeIdx(currEyeIdx), dx(2.0f / x) {}

        Camera(std::initializer_list<Vertex3f> e, unsigned int x = 1000, unsigned int y = 1000,
               unsigned int currEyeIdx = 0) : eyes(e),
                                              plane(x, std::vector<Pixel>(y)),
                                              eyeIdx(currEyeIdx), dx(2.0f / x) {}

        void render(const Scene &scene);

        void createImage(const std::string &filename) const;

    private:

        ColorDbl trace(Ray &ray, const Scene &scene, RNG &rng, int depth, ColorDbl &beta) const;

        ColorChar toneMap(ColorDbl c, double iMax) const;

        double getMaxPixelColorVal() const;

        const std::vector<Vertex3f> eyes;
        CameraPlane plane;

        unsigned int eyeIdx{0};
        const float dx{0.002};
        const int maxDepth = 10;

    };

}


#endif //RAYTRACER_CAMERA_H
