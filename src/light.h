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
        explicit Light(ColorDbl c) : L0(c) {}

    public:
        virtual ~Light() = default;
        // We use point lights -> only need position
        ColorDbl L0;
    };
}

#endif //RAYTRACER_LIGHT_H
