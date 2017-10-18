//
// Created by Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_VERTEX_H
#define RAYTRACER_VERTEX_H


#include "vector.h"

namespace rays {

    // Reasoning for a Vertex and a Vector class taken from PBRT

    template<typename T>
    class Vertex3 {
    public:

        Vertex3() = default;

        Vertex3(T x, T y, T z) : x(x), y(y), z(z) {}

        T x{0};
        T y{0};
        T z{0};

        template<typename U>
        explicit operator Vector3<U>() const {
            return Vector3<U>(x, y, z);
        }

        Vertex3<T> &operator+=(const Vertex3<T> &p) {
            x += p.x;
            y += p.y;
            z += p.z;
            return *this;
        }

        Vertex3<T> operator+(const Vertex3<T> &p) const {
            return Vertex3<T>(x + p.x, y + p.y, z + p.z);
        }

        Vertex3<T> operator+(const Vector3<T> &v) const {
            return Vertex3<T>(x + v.x, y + v.y, z + v.z);
        }

        Vertex3<T> &operator+=(const Vector3<T> &rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        Vector3<T> operator-(const Vertex3<T> &v) const {
            return Vector3<T>(x - v.x, y - v.y, z - v.z);
        }

        Vertex3<T> operator-(const Vector3<T> &v) const {
            return Vertex3<T>(x - v.x, y - v.y, z - v.z);
        }

        Vertex3<T> &operator-=(const Vector3<T> &v) {
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



    };

    template<typename T, typename U>
    inline Vertex3<T> operator*(U lhs, const Vertex3<T> &rhs) {
        return rhs * lhs;
    }
    // Vertex specifics

    template<typename T>
    inline float distance(const Vertex3<T> &p1, const Vertex3<T> &p2) {
        return (p1 - p2).length();
    }

    template<typename T>
    inline float distanceSquared(const Vertex3<T> &p1, const Vertex3<T> &p2) {
        return (p1 - p2).lengthSquared();
    }


    template<typename T>
    inline std::ostream& operator<<(std::ostream& os, const Vertex3<T> & v) {
        os << v.x << " " << v.y << " " << v.z;
        return os;
    }

    using Vertex3f = Vertex3<float>;

}
#endif //RAYTRACER_VERTEX_H
