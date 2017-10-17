//
// Created by Niclas Olmenius on 2017-09-08.
//

#ifndef RAYTRACER_SHAPE_H
#define RAYTRACER_SHAPE_H

#include "common.h"
#include "colordbl.h"
#include "direction.h"
#include "vertex.h"

namespace rays {

    struct Shape {

        virtual ~Shape() = default;

        virtual bool intersect(Ray &ray, IntersectionInfo *isect, float *tHit) const = 0;

        virtual Vertex3f getRandomPoint(RNG &rng) const = 0;

        virtual float area() const = 0;

    };

}

#endif //RAYTRACER_SHAPE_H
