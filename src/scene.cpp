//
// Created by Niclas Olmenius on 2017-09-06.
//

#include "scene.h"
#include "shape.h"
#include "ray.h"

namespace rays {

    bool Scene::intersect(Ray &ray) {

        for (auto &&shape : shapes) {
            if (shape.rayIntersection(ray)) {
                ray.endShape = shape;

            }

        }

        return false;

    }
}
