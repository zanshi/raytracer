//
// Created by Niclas Olmenius on 2017-09-07.
//

#ifndef RAYTRACER_TRIANGLEMESH_H
#define RAYTRACER_TRIANGLEMESH_H

#include "common.h"
#include "triangle.h"
#include "vertex.h"

namespace rays {

    struct TriangleMesh {

        TriangleMesh(std::vector<Triangle> triangles, std::vector<Vertex> vertices, std::vector<int> indices)
                : T(std::move(triangles)), V(std::move(vertices)), I(std::move(indices)) { }

        std::vector<Triangle> T;
        std::vector<Vertex> V;
        std::vector<int> I;


    };
}


#endif //RAYTRACER_TRIANGLEMESH_H
