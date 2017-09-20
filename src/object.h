//
// Created by Niclas Olmenius on 2017-09-13.
//

#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include "common.h"
#include "shape.h"
#include "material.h"

namespace rays {

    struct SceneObject {

        SceneObject(const std::vector<std::shared_ptr<Shape>> &shapes,
                    const std::vector<std::shared_ptr<Material>> &materials) : shapes(shapes), materials(materials) {}

        std::vector<std::shared_ptr<Shape>> shapes;
        std::vector<std::shared_ptr<Material>> materials;
    };

}
#endif //RAYTRACER_OBJECT_H
