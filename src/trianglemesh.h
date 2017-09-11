//
// Created by Niclas Olmenius on 2017-09-07.
//

#ifndef RAYTRACER_TRIANGLEMESH_H
#define RAYTRACER_TRIANGLEMESH_H

#include "common.h"
#include "vertex.h"

namespace rays {

    struct TriangleMesh {

        explicit TriangleMesh(std::unique_ptr<std::vector<Vertex>> vertices) : V(std::move(vertices)) { }

        TriangleMesh(std::unique_ptr<std::vector<Vertex>> vertices, std::unique_ptr<std::vector<int>> indices)
                : V(std::move(vertices)), I(std::move(indices)) {

        }

        std::unique_ptr<std::vector<Vertex>> V;
        std::unique_ptr<std::vector<int>> I;

    };
}


#endif //RAYTRACER_TRIANGLEMESH_H
