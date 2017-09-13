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

    public:

        explicit Scene(std::vector<std::shared_ptr<Shape>> meshes) : shapes{std::move(meshes)} {}

        std::vector<std::shared_ptr<Shape>> shapes;

        bool intersect(Ray &ray) const;

    };
}

#endif //RAYTRACER_SCENE_H
