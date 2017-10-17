//
// Created by Niclas Olmenius on 2017-09-24.
//

#ifndef RAYTRACER_RNG_H
#define RAYTRACER_RNG_H

#include "common.h"

namespace rays {

    class RNG {

    public:
        RNG() {
            gen = std::mt19937(rd());
            dis = std::uniform_real_distribution<float> (0, 1);
        }

        float getUniform1D() { return dis(gen); }

        std::pair<float, float> getUniform2D() { return std::make_pair(dis(gen), dis(gen)); };

    private:
        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen; //Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<float> dis;

    };
}

#endif //RAYTRACER_RNG_H
