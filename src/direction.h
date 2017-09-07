//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_DIRECTION_H
#define RAYTRACER_DIRECTION_H

namespace rays {
    struct Direction {
        using DataType = double;

        DataType x;
        DataType y;
        DataType z;
    };
}

#endif //RAYTRACER_DIRECTION_H
