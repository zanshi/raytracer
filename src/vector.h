//
// Created by Niclas Olmenius on 2017-09-13.
//

#ifndef RAYTRACER_VECTOR_H
#define RAYTRACER_VECTOR_H

#include <cassert>
#include "common.h"

namespace rays {

    template<typename T>
    struct Vector {

        Vector() = default;

        Vector(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

        T x{0};
        T y{0};
        T z{0};
        T w{0};

        Vector<T> &operator+=(const Vector<T> &rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            w += rhs.w;
            return *this;
        }

        friend Vector<T> operator+(Vector<T> lhs, const Vector<T> &rhs) {
            lhs += rhs;
            return lhs;
        }

        Vector<T> &operator-=(const Vector<T> &rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            w -= rhs.w;
            return *this;
        }

        friend Vector<T> operator-(Vector<T> lhs, const Vector<T> &rhs) {
            lhs -= rhs;
            return lhs;
        }

        Vector<T> &operator*=(const Vector<T> &rhs) {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;
            w *= rhs.w;
            return *this;
        }

        friend Vector<T> operator*(Vector<T> lhs, const Vector<T> &rhs) {
            lhs *= rhs;
            return lhs;
        }

        template<typename U>
        Vector<T> &operator*=(const U rhs) {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            w *= rhs;
            return *this;
        }

        template<typename U>
        Vector<T> operator*(U rhs) const {
           return Vector<T>(x * rhs, y * rhs, z * rhs, w * rhs); 
        }

        template<typename U>
        friend Vector<T> operator*(U lhs, const Vector<T>& rhs) {
            return rhs * lhs;
        }


        // Vector<T> specific operators

        T dot(const Vector<T> &rhs) const {
            return x * rhs.x + y * rhs.y + z * rhs.z;
        }

        Vector<T> cross(const Vector<T> &rhs) const {
            Vector<T> res{};
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

}
#endif //RAYTRACER_VECTOR_H
