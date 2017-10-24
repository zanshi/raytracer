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

#include <glm/fwd.hpp>

namespace rays {

    // Global forward declarations (reduces compilation times)
    class Camera;
    class Scene;
    class RNG;

    class SceneObject;
    struct Shape;
    struct Sphere;
    struct Triangle;
    struct TriangleMesh;

    struct IntersectionInfo;

    struct Ray;
    struct Pixel;
    struct ColorDbl;

    struct BSDF;
    struct Lambertian;
    struct OrenNayar;
    struct Glass;

    class Light;
    class AreaLight;


    // Useful constants
    constexpr const float PI = 3.14159265359f;
    constexpr const float invPI = 1.0f / PI;


	constexpr const float P = 0.75f;
	constexpr float invP = 1 / P;

    // Utility functions
    template<typename T>
    inline float clamp(const T &v, const T &lo, const T &hi) { return std::max(lo, std::min(hi, v)); }

    template <typename T>
    inline int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }






} // namespace rays


#endif //RAYTRACER_COMMON_H
