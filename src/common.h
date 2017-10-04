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
#include <random>

namespace rays {

    // Global forward declarations (reduces compilation times)
    template<typename T>
    struct Vector3;

    template<typename T>
    struct Vertex3;

    class Camera;
    class Scene;

    class SceneObject;
    struct Shape;
    struct Sphere;
    struct Triangle;
    struct TriangleMesh;

    struct IntersectionInfo;

    struct Ray;
    struct Pixel;
    struct ColorDbl;
    struct Material;
    struct BSDF;

    class Light;
    class PointLight;
    class AreaLight;

    constexpr const float M_PI = 3.14159265359f;
    constexpr const float invPI = 1.0f / M_PI;


} // namespace rays


#endif //RAYTRACER_COMMON_H
