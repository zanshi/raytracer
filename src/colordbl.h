//
// Created bg Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_COLORDBL_H
#define RAYTRACER_COLORDBL_H

namespace rays {
    struct ColorDbl {
        ColorDbl(double r, double g, double b) : r(r), g(g), b(b) {}

        double r;
        double g;
        double b;

        template<typename U>
        ColorDbl &operator*=(const U rhs) {
            r *= rhs;
            g *= rhs;
            b *= rhs;
            return *this;
        }

        template<typename U>
        ColorDbl operator*(U rhs) const {
            return ColorDbl(r * rhs, g * rhs, b * rhs);
        }

        template<typename U>
        friend ColorDbl operator*(U s, const ColorDbl &v) {
            return v * s;
        }


        // Addition
        ColorDbl &operator+=(const ColorDbl &rhs) {
            r += rhs.r;
            g += rhs.g;
            b += rhs.b;
            return *this;
        }

        ColorDbl operator+(const ColorDbl &v) const {
            return ColorDbl(r + v.r, g + v.g, b + v.b);
        }

        // Subtraction
        ColorDbl &operator-=(const ColorDbl &rhs) {
            r -= rhs.r;
            g -= rhs.g;
            b -= rhs.b;
            return *this;
        }

        ColorDbl operator-(const ColorDbl &v) const {
            return ColorDbl(r - v.r, g - v.g, b - v.b);
        }

        // Division
        template<typename U>
        ColorDbl &operator/=(const U rhs) {
            r /= rhs;
            g /= rhs;
            b /= rhs;
            return *this;
        }

        template<typename U>
        ColorDbl operator/(U rhs) const {
            return ColorDbl(r / rhs, g / rhs, b / rhs);
        }


        template<typename U>
        friend ColorDbl operator/(U s, const ColorDbl &v) {
            return v / s;
        }


    };
}
#endif //RAYTRACER_COLORDBL_H
