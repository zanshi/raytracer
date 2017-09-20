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

        // use arbitrary end?

        // x(t) = o + t(e - o)
        // get t with möller-trumbore
        // calc real intersection point with x(t)

        Ray() = default;

        Ray(const Vertex &origin, const Vertex &end)
                : o(origin), e(end) {}

        const Vertex o{0, 0, 0}; // origin
//        std::shared_ptr<Ray> parent;
//        std::shared_ptr<Ray> T;
//        std::shared_ptr<Ray> R;
        Vertex e{0, 0, 0}; // end
        //Direction d {0,0,0}; // normalized direction vector
        ColorDbl color{0, 0, 0};
        Vector3f intersectionNormal;
        std::shared_ptr<Shape> endShape{nullptr};
    };
}
#endif //RAYTRACER_RAY_H
