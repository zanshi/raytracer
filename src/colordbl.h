//
// Created bg Niclas Olmenius on 2017-09-06.
//

#ifndef RAYTRACER_COLORDBL_H
#define RAYTRACER_COLORDBL_H

#include <sstream>

namespace rays {
    struct ColorDbl {

        ColorDbl() : r(0), g(0), b(0) {}
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

        ColorDbl operator*(const ColorDbl &rhs) const {
            return ColorDbl(r * rhs.r, g * rhs.g, b * rhs.b);
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

        std::string toString() const {
            std::stringstream ss;
            ss << r << " " << g << " " << b;
            return ss.str();
        }

        double maxColorVal(double maxVal) const {
            maxVal = std::max(r, maxVal);
            maxVal = std::max(g, maxVal);
            maxVal = std::max(b, maxVal);
            return maxVal;
        }

    };

    inline ColorDbl gammaCorrect(const ColorDbl &c) {
        return ColorDbl(std::pow(c.r, 1.0 / 2.2), std::pow(c.g, 1.0 / 2.2), std::pow(c.b, 1.0 / 2.2));
    }

    template<typename T>
    inline ColorDbl clamp(const ColorDbl&c, T min, T max) {
        return ColorDbl(clamp(c.r, min, max), clamp(c.g, min, max), clamp(c.b, min, max));
    }

    inline std::ostream &operator<<(std::ostream &os, const ColorDbl &v) {
        return os << v.toString();
    }

    template<typename U>
    inline ColorDbl operator/(U s, const ColorDbl &v) {
        return v / s;
    }

    template<typename U>
    inline ColorDbl operator*(U s, const ColorDbl &v) {
        return v * s;
    }
}
#endif //RAYTRACER_COLORDBL_H
