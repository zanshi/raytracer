//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include "common.h"
#include "triangle.h"
#include "trianglemesh.h"

namespace rays {
    class Scene {

        explicit Scene(std::vector<Shape> meshes) : shapes{std::move(meshes)} {}

        std::vector<Shape> shapes;

        bool intersect(Ray &ray);

    };
}

#endif //RAYTRACER_SCENE_H
