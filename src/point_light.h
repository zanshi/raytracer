//
// Created by Niclas Olmenius on 2017-09-15.
//

#ifndef RAYTRACER_POINT_LIGHT_H
#define RAYTRACER_POINT_LIGHT_H

#include "common.h"
#include "light.h"

namespace rays {
    struct PointLight final : public Light {
        PointLight() : Light() {}
        PointLight(Vertex p, ColorDbl c, double i) : Light(p, c, i) {}
    };
}

#endif //RAYTRACER_POINT_LIGHT_H
