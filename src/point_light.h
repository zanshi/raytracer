//
// Created by Niclas Olmenius on 2017-09-15.
//

#ifndef RAYTRACER_POINT_LIGHT_H
#define RAYTRACER_POINT_LIGHT_H

#include "common.h"
#include "light.h"

namespace rays {
    class PointLight final : public Light {
    public:
        explicit PointLight(ColorDbl c, Vertex3f pos) : Light(c), pos(pos) {}

        Vertex3f pos{};

    };
}

#endif //RAYTRACER_POINT_LIGHT_H
