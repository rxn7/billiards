#pragma once

#include <cassert>
#include <algorithm>
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

namespace MathUtils {
    inline float length(const sf::Vector2f &v) {
        return std::sqrt(v.x*v.x + v.y*v.y);
    }

    inline float length(const sf::Vector3f &v) {
        return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    }

    inline float lengthSqr(const sf::Vector2f &v) {
        return v.x*v.x + v.y*v.y;
    }

    template<typename T> 
    T normalized(const T &v) {
        float len = length(v);
        if(len == 0) return v;
        return v / len;
    }

    inline sf::Vector3f normalized(const sf::Vector3f &v) {
        float len = length(v);
        if(len == 0) return v;
        return v / len;
    }

    inline sf::Vector3f cross(const sf::Vector3f &a, const sf::Vector3f &b) {
        return sf::Vector3f(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }
}
