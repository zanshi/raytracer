//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_PIXEL_H
#define RAYTRACER_PIXEL_H

#include "common.h"
#include "colordbl.h"

namespace rays {

    struct Pixel {
        ColorDbl color{0, 0, 0};
        std::vector<std::shared_ptr<Ray>> rays{nullptr};

//    friend bool operator<(const Pixel& lhs, const Pixel& rhs) {
//        return (lhs.color.r < rhs.color.r) && (lhs.color.g < rhs.color.g) && (lhs.color.b < rhs.color.b);
//    }
    };

}
#endif //RAYTRACER_PIXEL_H
