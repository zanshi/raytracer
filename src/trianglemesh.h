//
// Created by Niclas Olmenius on 2017-09-07.
//

#ifndef RAYTRACER_TRIANGLEMESH_H
#define RAYTRACER_TRIANGLEMESH_H

#include "common.h"

namespace rays {

    struct TriangleMesh {

        explicit TriangleMesh(std::vector<glm::vec3> vertices) : V(std::move(vertices)) {}

//        TriangleMesh(std::unique_ptr<std::vector<glm::vec3>> vertices, std::unique_ptr<std::vector<int>> indices)
//                : V(std::move(vertices)), I(std::move(indices)) {
//
//        }

        std::vector<glm::vec3> V;
        std::vector<int> I;
        // color vector?

    };
}


#endif //RAYTRACER_TRIANGLEMESH_H
