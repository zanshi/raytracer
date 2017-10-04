//
// Created by Niclas Olmenius on 2017-09-15.
//

#ifndef RAYTRACER_LIGHT_H
#define RAYTRACER_LIGHT_H

#include "common.h"
#include "vertex.h"
#include "colordbl.h"

namespace rays {
    class Light {

    protected:
        explicit Light(ColorDbl c) : color(c) {}

    public:
        // We use point lights -> only need position
        ColorDbl color;
    };
}

#endif //RAYTRACER_LIGHT_H
