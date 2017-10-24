//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "common.h"
#include "rng.h"


namespace rays {

    using ColorChar = std::array<unsigned char, 3>;

    class Camera {
        using CameraPlane = std::vector<std::vector<ColorDbl>>;
    public:

        explicit Camera(std::vector<glm::vec3> eyes, unsigned int x, unsigned int y, unsigned int currEyeIdx,
                        unsigned int nSamples)
                : eyes(std::move(eyes)),
                  plane(x, std::vector<ColorDbl>(y)),
                  eyeIdx(currEyeIdx), nSamples(nSamples), dx(2.0f / x) {}

        void render(const Scene &scene);

        void createImage(const std::string &filename) const;

    private:

        ColorDbl trace(const Ray &ray, const Scene &scene, RNG &rng, int depth, bool specularBounce) const;

        ColorChar toneMap(ColorDbl c, double iMax) const;

        double getMaxPixelColorVal() const;

        const std::vector<glm::vec3> eyes;
        CameraPlane plane;

        const unsigned int eyeIdx{0};
        const unsigned int nSamples = 64;
        const float dx{0.002f};
        const int maxDepth = 7;

    };

}


#endif //RAYTRACER_CAMERA_H
