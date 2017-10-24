//
// Created by Niclas Olmenius on 2017-09-15.
//

#ifndef RAYTRACER_LIGHT_H
#define RAYTRACER_LIGHT_H

#include "common.h"
#include "colordbl.h"

namespace rays {
    class Light {

    protected:
        explicit Light(ColorDbl c, float i) : L0(c), intensity(i) {}

    public:
        virtual ~Light() = default;
        // We use point lights -> only need position
        ColorDbl L0;
        float intensity;
    };
}

#endif //RAYTRACER_LIGHT_H
