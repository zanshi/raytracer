//
// Created by Niclas Olmenius on 2017-09-24.
//

#ifndef RAYTRACER_RNG_H
#define RAYTRACER_RNG_H

#include <chrono>
#include "common.h"
#include <glm/glm.hpp>

namespace rays {

    class RNG {

    public:
        RNG() : gen{
                static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())},
                dis{0, 1} {}

        float getUniform1D() { return dis(gen); }

        glm::vec2 getUniform2D() { return glm::vec2(dis(gen), dis(gen)); };

    private:
        std::mt19937 gen; //Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<float> dis;

    };
}

#endif //RAYTRACER_RNG_H
