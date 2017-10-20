//
// Created by Niclas Olmenius on 2017-09-24.
//

#ifndef RAYTRACER_RNG_H
#define RAYTRACER_RNG_H

#include <ctime>
#include <chrono>
#include "common.h"

namespace rays {

    class RNG {

    public:
        RNG() : gen{
                static_cast<unsigned long long int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())}, dis{0, 1} {}

        float getUniform1D() { return dis(gen); }

        std::pair<float, float> getUniform2D() { return std::make_pair(dis(gen), dis(gen)); };

    private:
        std::mt19937_64 gen; //Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<float> dis;

    };
}

#endif //RAYTRACER_RNG_H
