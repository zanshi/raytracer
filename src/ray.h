//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "common.h"
#include "vector.h"
#include "vertex.h"
#include "colordbl.h"
#include "direction.h"

namespace rays {
    struct Ray {

        // start -> end = (end - origin) forms a direction
        // many rays spawn from the same start vertex -> use ref/ptr
        // the end vertex is the start vertex for new rays -> ref/ptr
        // save 3 * 8 - 8 = 16 bytes

        explicit Ray(const Vertex3f &o, const Vector3f &d) : o(o), d(d) {}

        const Vertex3f o{0, 0, 0}; // origin
        Vector3f d;
    };
}
#endif //RAYTRACER_RAY_H
