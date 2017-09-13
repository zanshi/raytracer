//
// Created by Niclas Olmenius on 2017-09-06.
//

#include "scene.h"
#include "shape.h"
#include "sphere.h"
#include "ray.h"

namespace rays {

    bool Scene::intersect(Ray &ray) const {
        // Check for intersection with every shape in the scene
        for (auto &&shape : shapes) {
            // Update the ray values if it hit a shape
            if (shape->rayIntersection(ray)) {
                ray.endShape = shape;
                ray.color = shape->color; // Replace with BDRF later on?
                return true;
            }
        }

        return false;

    }
}
