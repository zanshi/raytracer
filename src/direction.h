//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_DIRECTION_H
#define RAYTRACER_DIRECTION_H


#include <Eigen/Dense>

namespace rays {

    using Direction = Eigen::Matrix<float, 3, 1>;

//    struct Direction {
//        using DataType = double;
//
//        DataType x;
//        DataType y;
//        DataType z;
//    };
}

#endif //RAYTRACER_DIRECTION_H
