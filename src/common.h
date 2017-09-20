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
#include <cmath>


namespace rays {

    // Global forward declarations (reduces compilation times)
    template<typename T>
    struct Vector3;

    template<typename T>
    struct Vertex3;

    class Camera;
    class Scene;

    struct SceneObject;
    struct Shape;
    struct Sphere;
    struct Triangle;
    struct TriangleMesh;

    struct Ray;
    struct Pixel;
    struct ColorDbl;
    struct Material;

    class Light;
    class PointLight;
    class AreaLight;


} // namespace rays


#endif //RAYTRACER_COMMON_H
