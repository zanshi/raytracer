//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "common.h"
#include "vector.h"

namespace rays {
    struct Ray {

        // start -> end = (end - origin) forms a direction
        // many rays spawn from the same start vertex -> use ref/ptr
        // the end vertex is the start vertex for new rays -> ref/ptr
        // save 3 * 8 - 8 = 16 bytes

        explicit Ray(const glm::vec3 &o, const glm::vec3 &d) : o(o), d(d), tMax(std::numeric_limits<float>::max()) {}

        const glm::vec3 o{0, 0, 0}; // origin
        const glm::vec3 d;
        mutable float tMax;
    };
}
#endif //RAYTRACER_RAY_H
