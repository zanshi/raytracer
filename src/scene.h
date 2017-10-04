//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include "common.h"
#include "light.h"
#include "sceneobject.h"

namespace rays {
    class Scene {

    public:

        explicit Scene(std::vector<std::shared_ptr<SceneObject>> s, std::vector<std::shared_ptr<Light>> l)
                : objects{std::move(s)}, lights{std::move(l)} {}

        bool intersect(Ray &ray, IntersectionInfo *isect) const;

        std::vector<std::shared_ptr<SceneObject>> objects{nullptr};
        std::vector<std::shared_ptr<Light>> lights{};

    };
}

#endif //RAYTRACER_SCENE_H
