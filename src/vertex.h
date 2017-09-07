//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_VERTEX_H
#define RAYTRACER_VERTEX_H

//#include <Eigen/Dense>

namespace rays {

    //using Vertex = Eigen::Vector4d;

    struct Vertex {
        using DataType = double;

        DataType x;
        DataType y;
        DataType z;
        DataType w;
    };

}
#endif //RAYTRACER_VERTEX_H
