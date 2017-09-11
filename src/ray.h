//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "common.h"
#include "vertex.h"
#include "colordbl.h"
#include "triangle.h"
#include "direction.h"

#include <Eigen/Dense>

namespace rays {
    struct Ray {

        std::shared_ptr<Vertex> start;
        std::shared_ptr<Vertex> end;
        Vertex o; // origin
        Direction d; // normalized direction vector
        Shape& endShape;
        ColorDbl rayColor;
    };
}
#endif //RAYTRACER_RAY_H
