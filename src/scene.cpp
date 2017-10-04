//
// Created by Niclas Olmenius on 2017-09-06.
//

#include "scene.h"
#include "ray.h"
#include "intersectioninfo.h"

namespace rays {

    bool Scene::intersect(Ray &ray, IntersectionInfo *isect) const {
        // Check for intersection with every object in the scene
        bool hit = false;
        for (auto &o : objects) {
            if (o->intersect(ray, isect)) {
                hit = true;
            }
        }

        return hit;
    }
}
