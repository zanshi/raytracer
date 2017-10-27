//
// Created by Niclas Olmenius on 2017-09-20.
//

#ifndef RAYTRACER_AREA_LIGHT_H
#define RAYTRACER_AREA_LIGHT_H

#include "common.h"
#include "light.h"
#include "rng.h"

#include "shape.h"

namespace rays {
    class AreaLight final : public Light {
    public:
        explicit AreaLight(const ColorDbl &c, const std::shared_ptr<Shape> &shape, float i)
                : Light(c, i), T(shape), area(shape->area()) {}

        ~AreaLight() override = default;

        const std::shared_ptr<Shape> T;
        const float area;

    };

}

#endif //RAYTRACER_AREA_LIGHT_H
