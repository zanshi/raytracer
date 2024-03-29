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

        explicit Scene(std::vector<SceneObject> s,
                       std::vector<std::shared_ptr<AreaLight>> l)
                : objects{std::move(s)}, lights{std::move(l)} {}

        bool intersect(Ray &ray, IntersectionInfo *isect) const;

        const std::vector<SceneObject> objects;
        const std::vector<std::shared_ptr<AreaLight>> lights{nullptr};

    };
}

#endif //RAYTRACER_SCENE_H
