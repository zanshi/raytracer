//
// Created by Niclas Olmenius on 2017-09-20.
//

#include "common.h"
#include "sceneobject.h"
#include "ray.h"
#include "intersectioninfo.h"

namespace rays {

    bool SceneObject::intersect(const Ray &ray, IntersectionInfo *isect) const {

        // tHit is used to get the closest intersection point
        float tHit;

        if (!(shape->intersect(ray, isect, &tHit))) { return false; }

        ray.tMax = tHit;
        isect->obj = this;
        isect->brdf = bsdf.get();

        return true;

    }

    const AreaLight *SceneObject::getAreaLight() const {
        return areaLight.get();
    }

}
