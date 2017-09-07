//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "common.h"
#include "vertex.h"
#include "colordbl.h"
#include "triangle.h"

namespace rays {
    struct Ray {

        std::shared_ptr<Vertex> start;
        std::shared_ptr<Vertex> end;
        std::shared_ptr<Triangle> endTriangle;
        ColorDbl rayColor;
    };
}
#endif //RAYTRACER_RAY_H
