//
// Created by Niclas Olmenius on 2017-09-08.
//

#ifndef RAYTRACER_SHAPE_H
#define RAYTRACER_SHAPE_H

#include "common.h"
#include "colordbl.h"
#include <glm/vec2.hpp>

namespace rays {

    class Shape {
    public:
        virtual ~Shape() = default;

        virtual bool intersect(const Ray &ray, IntersectionInfo *isect, float *tHit) const = 0;

        virtual glm::vec3 getRandomPoint(glm::vec2 r2) const = 0;

        virtual float area() const = 0;

    };

}

#endif //RAYTRACER_SHAPE_H
