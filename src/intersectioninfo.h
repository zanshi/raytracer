//
// Created by Niclas Olmenius on 2017-09-20.
//

#ifndef RAYTRACER_INTERSECTIONINFO_H
#define RAYTRACER_INTERSECTIONINFO_H

#include "common.h"
#include <glm/glm.hpp>
#include "bsdf.h"
#include "shape.h"
#include "sceneobject.h"
#include "bsdf.h"

namespace rays {
    struct IntersectionInfo {
        IntersectionInfo() = default;

        IntersectionInfo(const glm::vec3 &p, const glm::vec3 &wo, const glm::vec3 &n, const Shape *shape) : p(p), wo(wo),
                                                                                                        n(n),
                                                                                                        shape(shape) {}

        glm::vec3 p;
        glm::vec3 wo;
        glm::vec3 n;
        const Shape *shape = nullptr;
        const SceneObject *obj = nullptr;
        BSDF *brdf = nullptr;
    };

}

#endif //RAYTRACER_INTERSECTIONINFO_H
