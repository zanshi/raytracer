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
        const ColorDbl L0;
        const float intensity;
    };
}

#endif //RAYTRACER_LIGHT_H
