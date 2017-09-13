//
// Created by Niclas Olmenius on 2017-09-08.
//

#ifndef RAYTRACER_SHAPE_H
#define RAYTRACER_SHAPE_H

#include "common.h"
#include "colordbl.h"

namespace rays {

    struct Shape {
        explicit Shape(ColorDbl c) : color(c) {}

        virtual ~Shape() = default;

        virtual bool rayIntersection(Ray &ray) const = 0;

        ColorDbl color;
    };

}

#endif //RAYTRACER_SHAPE_H
