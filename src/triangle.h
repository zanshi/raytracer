//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "common.h"
#include "colordbl.h"
#include "direction.h"
#include "vertex.h"

namespace rays {
    struct Triangle {

       // explicit Triangle(std::array<Vertex, 3> verts, Direction n, ColorDbl c)
       //             : vertices(verts), normal(n), color(c) {}

        std::array<Vertex, 3> vertices;
        Direction normal;
        ColorDbl color;

        Vertex rayIntersection(const Ray &arg);
    };
}
#endif //RAYTRACER_TRIANGLE_H
