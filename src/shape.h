//
// Created by Niclas Olmenius on 2017-09-08.
//

#ifndef RAYTRACER_SHAPE_H
#define RAYTRACER_SHAPE_H

#include "common.h"
#include "colordbl.h"

namespace rays {

    struct Shape {

        virtual ~Shape() = default;

        virtual bool intersect(const Ray &ray, IntersectionInfo *isect, float *tHit) const = 0;

        virtual glm::vec3 getRandomPoint(RNG &rng) const = 0;

        virtual float area() const = 0;

    };

}

#endif //RAYTRACER_SHAPE_H
