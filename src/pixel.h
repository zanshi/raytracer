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
    };

}
#endif //RAYTRACER_PIXEL_H
