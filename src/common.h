//
// Created by Niclas Olmenius on 2017-09-07.
//

#ifndef RAYTRACER_COMMON_H
#define RAYTRACER_COMMON_H

// Global includes
#include <vector>
#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <algorithm>
#include <utility>
#include <initializer_list>


namespace rays {

    // Global forward declarations
    template<typename T>
    struct Vector;

    class Camera;

    class Scene;

    struct Pixel;
    struct Shape;
    struct Sphere;
    struct Triangle;
    struct TriangleMesh;
    //struct Direction;
    struct Ray;
    struct ColorDbl;


} // namespace rays


#endif //RAYTRACER_COMMON_H
