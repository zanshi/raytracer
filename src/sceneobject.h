//
// Created by Niclas Olmenius on 2017-09-13.
//

#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include "common.h"
#include "shape.h"
#include "material.h"
#include "area_light.h"

namespace rays {

    class SceneObject {
    public:
        explicit SceneObject(const std::shared_ptr<Shape> &s, const std::shared_ptr<Material> &m,
                    const std::shared_ptr<AreaLight> &l = nullptr)
                : shape(s), material(m), areaLight(l) {}

        bool intersect(Ray &ray, IntersectionInfo *isect) const;

        const std::shared_ptr<Shape> shape;
        const std::shared_ptr<Material> material;
        const std::shared_ptr<AreaLight> areaLight{nullptr};

    };

}
#endif //RAYTRACER_OBJECT_H
