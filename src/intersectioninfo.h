//
// Created by Niclas Olmenius on 2017-09-20.
//

#ifndef RAYTRACER_INTERSECTIONINFO_H
#define RAYTRACER_INTERSECTIONINFO_H

#include "common.h"
#include "vertex.h"
#include "vector.h"
#include "bsdf.h"
#include "shape.h"
#include "sceneobject.h"
#include "bsdf.h"

namespace rays {
    struct IntersectionInfo {
        IntersectionInfo() = default;

        IntersectionInfo(const Vertex3f &p, const Vector3f &wo, const Vector3f &n, const Shape *shape) : p(p), wo(wo),
                                                                                                        n(n),
                                                                                                        shape(shape) {}

        Vertex3f p{};
        Vector3f wo{};
        Vector3f n{};
        const Shape *shape = nullptr;
        const SceneObject *obj = nullptr;
        BSDF *brdf = nullptr;
    };

}

#endif //RAYTRACER_INTERSECTIONINFO_H
