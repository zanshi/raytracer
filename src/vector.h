//
// Created by Niclas Olmenius on 2017-09-13.
//

#ifndef RAYTRACER_VECTOR_H
#define RAYTRACER_VECTOR_H

#include <cassert>
#include <math.h>
#include "common.h"

namespace rays {

    template<typename T>
    class Vector3 {

    public:
        Vector3() = default;

        Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

        T x{0};
        T y{0};
        T z{0};


        // Addition
        Vector3<T> &operator+=(const Vector3<T> &rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        Vector3<T> operator+(const Vector3<T> &v) const {
            return Vector3<T>(x + v.x, y + v.y, z + v.z);
        }

        // Subtraction
        Vector3<T> &operator-=(const Vector3<T> &rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }

        Vector3<T> operator-(const Vector3<T> &v) const {
            return Vector3<T>(x - v.x, y - v.y, z - v.z);
        }


        // Multiplication
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

        // Division
        template<typename U>
        Vector3<T> &operator/=(const U rhs) {
            x /= rhs;
            y /= rhs;
            z /= rhs;
            return *this;
        }

        template<typename U>
        Vector3<T> operator/(U rhs) const {
            return Vector3<T>(x / rhs, y / rhs, z / rhs);
        }

        // Negate
        Vector3<T> operator-() const {
            return Vector3<T>(-x, -y, -z);
        }

        // Vector specific operators

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

        float length() const {
            return std::sqrt(lengthSquared());
        }

        float lengthSquared() const {
            return x * x + y * y + z * z;
        }




    };

    template <typename T>
    inline Vector3<T> faceforward(const Vector3<T> &v, const Vector3<T> &v2) {
        return (v.dot(v2) < 0.f) ? -v : v;
    }

    template<typename T>
    Vector3<T> normalize(const Vector3<T> &v) {
        return v / v.length();
    }

    template<typename T, typename U>
    inline Vector3<T> operator/(U s, const Vector3<T> &v) {
        return v / s;
    }

    template<typename T, typename U>
    inline Vector3<T> operator*(U s, const Vector3<T> &v) {
        return v * s;
    }

    template<typename T>
    inline std::ostream& operator<<(std::ostream& os, const Vector3<T> & v) {
        os << v.x << " " << v.y << " " << v.z;
        return os;
    }

    using Vector3f = Vector3<float>;
}
#endif //RAYTRACER_VECTOR_H
