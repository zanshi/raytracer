//
// Created by Niclas Olmenius on 2017-09-13.
//

#ifndef RAYTRACER_VECTOR_H
#define RAYTRACER_VECTOR_H

#include <cassert>
#include "common.h"

namespace rays {

    template<typename T>
    struct Vector4 {

        Vector4() = default;

        Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

        T x{0};
        T y{0};
        T z{0};
        T w{0};

        Vector4<T> &operator+=(const Vector4<T> &rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            w += rhs.w;
            return *this;
        }

        friend Vector4<T> operator+(Vector4<T> lhs, const Vector4<T> &rhs) {
            lhs += rhs;
            return lhs;
        }

        Vector4<T> &operator-=(const Vector4<T> &rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            w -= rhs.w;
            return *this;
        }

        friend Vector4<T> operator-(Vector4<T> lhs, const Vector4<T> &rhs) {
            lhs -= rhs;
            return lhs;
        }

        Vector4<T> &operator*=(const Vector4<T> &rhs) {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;
            w *= rhs.w;
            return *this;
        }

        friend Vector4<T> operator*(Vector4<T> lhs, const Vector4<T> &rhs) {
            lhs *= rhs;
            return lhs;
        }

        template<typename U>
        Vector4<T> &operator*=(const U rhs) {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            w *= rhs;
            return *this;
        }

        template<typename U>
        Vector4<T> operator*(U rhs) const {
            return Vector4<T>(x * rhs, y * rhs, z * rhs, w * rhs);
        }

        template<typename U>
        friend Vector4<T> operator*(U lhs, const Vector4<T> &rhs) {
            return rhs * lhs;
        }


        // Vector4<T> specific operators

        T dot(const Vector4<T> &rhs) const {
            return x * rhs.x + y * rhs.y + z * rhs.z;
        }

        Vector4<T> cross(const Vector4<T> &rhs) const {
            Vector4<T> res{};
            // This currently assumes that w in both *this and rhs is 1.0
            // should probably make it more general
            //assert(std::abs(1.0 - w) < std::numeric_limits<T>::epsilon());
            //assert(std::abs(1.0 - rhs.w) < std::numeric_limits<T>::epsilon());

            res.x = y * rhs.z - z * rhs.y;
            res.y = z * rhs.x - x * rhs.z;
            res.z = x * rhs.y - y * rhs.x;

            return res;
        }


    };

    template<typename T>
    struct Vector3 {

        Vector3() = default;

        Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

        T x{0};
        T y{0};
        T z{0};

        Vector3<T> &operator+=(const Vector3<T> &rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        friend Vector3<T> operator+(Vector3<T> lhs, const Vector3<T> &rhs) {
            lhs += rhs;
            return lhs;
        }

        Vector3<T> &operator-=(const Vector3<T> &rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }

        friend Vector3<T> operator-(Vector3<T> lhs, const Vector3<T> &rhs) {
            lhs -= rhs;
            return lhs;
        }

        Vector3<T> &operator*=(const Vector3<T> &rhs) {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;
            return *this;
        }

        friend Vector3<T> operator*(Vector3<T> lhs, const Vector3<T> &rhs) {
            lhs *= rhs;
            return lhs;
        }

        template<typename U>
        Vector3<T> &operator*=(const U rhs) {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            return *this;
        }

        template<typename U>
        Vector3<T> operator*(U rhs) const {
            return Vector3<T>(x * rhs, y * rhs, z * rhs);
        }

        template<typename U>
        friend Vector3<T> operator*(U lhs, const Vector3<T> &rhs) {
            return rhs * lhs;
        }


        // Vector3<T> specific operators

        T dot(const Vector3<T> &rhs) const {
            return x * rhs.x + y * rhs.y + z * rhs.z;
        }

        Vector3<T> cross(const Vector3<T> &rhs) const {
            Vector3<T> res{};

            res.x = y * rhs.z - z * rhs.y;
            res.y = z * rhs.x - x * rhs.z;
            res.z = x * rhs.y - y * rhs.x;

            return res;
        }


    };
}
#endif //RAYTRACER_VECTOR_H
