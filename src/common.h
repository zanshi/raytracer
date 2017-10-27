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

//#define GLM_FORCE_CXX14
#define GLM_FORCE_SSE42 // or GLM_FORCE_SSE42 if your processor supports it
#define GLM_FORCE_ALIGNED
#define GLM_FORCE_EXPLICIT_CTOR

#include <glm/fwd.hpp>

namespace rays {

    // Global forward declarations (reduces compilation times)
    class Camera;
    class Scene;
    class SceneBuilder;
    class RNG;

    class SceneObject;
    class Shape;
    class Sphere;
    class Triangle;

    struct IntersectionInfo;

    struct Ray;
    struct ColorDbl;

    struct BSDF;
    struct Lambertian;
    struct OrenNayar;
    struct Glass;
    struct Mirror;

    class Light;
    class AreaLight;


    // Useful constants
    constexpr float PI = 3.1415926535897932384626433832795f;
    constexpr float PIx2 = PI * 2;
    constexpr float invPI = 1 / PI;


	constexpr float P = 0.75f;
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
