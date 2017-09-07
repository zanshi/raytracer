//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include "common.h"
#include "triangle.h"

namespace rays {
    class Scene {

        explicit Scene(std::vector<Triangle> aggregates) : triangles{std::move(aggregates)} {}

        std::vector<Triangle> triangles;

        Triangle &intersectedTriangle(const Ray &ray);

    };
}

#endif //RAYTRACER_SCENE_H
