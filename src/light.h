//
// Created by Niclas Olmenius on 2017-09-15.
//

#ifndef RAYTRACER_LIGHT_H
#define RAYTRACER_LIGHT_H

#include "vertex.h"
#include "colordbl.h"

namespace rays {
    struct Light {
        Light() = default;
        Light(Vertex p, ColorDbl c, double i) : pos(p), color(c), intensity(i) {}

        // We use point lights -> only need position
        Vertex pos{};
        ColorDbl color{};
        double intensity{};
    };
}

#endif //RAYTRACER_LIGHT_H
