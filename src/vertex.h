//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_VERTEX_H
#define RAYTRACER_VERTEX_H


#include "vector.h"

namespace rays {

    // Reasoning for a Vertex and a Vector class taken from PBRT

    template<typename T>
    struct Vertex3 {

        Vertex3() = default;

        Vertex3(T x, T y, T z) : x(x), y(y), z(z) {}

        T x{0};
        T y{0};
        T z{0};

        Vertex3<T> operator+(const Vector3 <T> &v) const {
            return Vertex3<T>(x + v.x, y + v.y, z + v.z);
        }

        Vertex3<T> &operator+=(const Vector3 <T> &rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        Vector3 <T> operator-(const Vertex3<T> &v) const {
            return Vector3<T>(x - v.x, y - v.y, z - v.z);
        }

        Vertex3<T> operator-(const Vector3 <T> &v) const {
            return Vertex3<T>(x - v.x, y - v.y, z - v.z);
        }

        Vertex3<T> &operator-=(const Vector3 <T> &v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        template<typename U>
        Vertex3<T> &operator*=(const U rhs) {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            return *this;
        }

        template<typename U>
        Vertex3<T> operator*(U rhs) const {
            return Vertex3<T>(x * rhs, y * rhs, z * rhs);
        }

        template<typename U>
        friend Vertex3<T> operator*(U lhs, const Vertex3<T> &rhs) {
            return rhs * lhs;
        }


        // Vertex specifics

        friend float distance(const Vertex3<T> &p1, const Vertex3<T> &p2) {
            return (p1 - p2).length();
        }

        friend float distanceSquared(const Vertex3<T> &p1, const Vertex3<T> &p2) {
            return (p1 - p2).lengthSquared();
        }


    };

    using Vertex3f = Vertex3<float>;

}
#endif //RAYTRACER_VERTEX_H
