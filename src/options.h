//
// Created by niclas on 2017-10-26.
//

#ifndef RAYTRACER_OPTIONS_H
#define RAYTRACER_OPTIONS_H

#include "common.h"
#include <glm/vec3.hpp>

namespace rays {
    namespace options {
        static constexpr int width = 256;
        static constexpr float dx = 2.0f / width;
        static constexpr int nrSamples = 128;
        static constexpr int nrLightSamples = 1;
        static constexpr int eyeIdx = 1;
        static constexpr float epsilon = 1e-5f;
        static constexpr int maxDepth = 7;
    }
}

#endif //RAYTRACER_OPTIONS_H
