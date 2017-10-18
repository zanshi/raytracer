//
// Created by Niclas Olmenius on 2017-09-20.
//

#include "common.h"
#include "sceneobject.h"
#include "ray.h"
#include "intersectioninfo.h"

namespace rays {

    bool SceneObject::intersect(Ray &ray, IntersectionInfo *isect) const {

        // tHit is used to get the closest intersection point
        float tHit = std::numeric_limits<float>::max();

        if (!(shape->intersect(ray, isect, &tHit))) { return false; }

//        ray.t = tHit;
        isect->obj = this;
        isect->brdf = bsdf.get();

        return true;

    }

    const AreaLight *SceneObject::getAreaLight() const {
            return areaLight.get();
    }

}
