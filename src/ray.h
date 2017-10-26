//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "common.h"
#include "vector.h"

namespace rays {
    struct Ray {

        explicit Ray(const glm::vec3 &o, const glm::vec3 &d) : o(o), d(d) {}

        const glm::vec3 o; // origin
        const glm::vec3 d;
        mutable float tMax = std::numeric_limits<float>::max();
    };


    inline std::ostream& operator<<(std::ostream& os, const Ray& r) {
        os << "Position: (" << r.o.x << ", " << r.o.y << ", " << r.o.z << "), "
                << "Direction: (" << r.d.x << ", " << r.d.y << ", " << r.d.z << ")";
        return os;
    }
}
#endif //RAYTRACER_RAY_H
