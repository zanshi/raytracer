//
// Created by Niclas Olmenius on 2017-09-13.
//

#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include "common.h"
#include "shape.h"
#include "bsdf.h"
#include "area_light.h"

namespace rays {

    class SceneObject {
    public:
        explicit SceneObject(const std::shared_ptr<Shape> &s, const std::shared_ptr<BSDF> &b,
                    const std::shared_ptr<AreaLight> &l = nullptr)
                : shape(s), bsdf(b), areaLight(l) {}

        bool intersect(Ray &ray, IntersectionInfo *isect) const;

        const AreaLight* getAreaLight() const;

        const std::shared_ptr<Shape> shape;
        const std::shared_ptr<BSDF> bsdf;
        std::shared_ptr<AreaLight> areaLight{nullptr};

    };

}
#endif //RAYTRACER_OBJECT_H
