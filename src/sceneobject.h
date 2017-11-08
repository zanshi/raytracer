//
// Created by Niclas Olmenius on 2017-09-13.
//

#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include <utility>

#include "common.h"
#include "shape.h"
#include "bsdf.h"
#include "area_light.h"

namespace rays {

    class SceneObject {
    public:
        explicit SceneObject(std::shared_ptr<Shape> s, std::shared_ptr<BSDF> b,
                             std::shared_ptr<AreaLight> l = nullptr)
                : shape(std::move(s)), bsdf(std::move(b)), areaLight(std::move(l)) {}

        ~SceneObject() = default;

        bool intersect(const Ray &ray, IntersectionInfo *isect) const;

        const AreaLight *getAreaLight() const;

        std::shared_ptr<Shape> shape;
        std::shared_ptr<BSDF> bsdf;
        std::shared_ptr<AreaLight> areaLight;

    };

}
#endif //RAYTRACER_OBJECT_H
