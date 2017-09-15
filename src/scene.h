//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include "common.h"
#include "shape.h"
#include "light.h"

namespace rays {
    class Scene {

    public:

        explicit Scene(std::vector<std::shared_ptr<Shape>> s, std::vector<Light> l) : shapes{std::move(s)},
                                                                                      lights{std::move(l)} {}

        bool intersect(Ray &ray) const;

        std::vector<std::shared_ptr<Shape>> shapes{nullptr};
        std::vector<Light> lights{};

    };
}

#endif //RAYTRACER_SCENE_H
