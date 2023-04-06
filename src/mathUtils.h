#pragma once

#include <cassert>
#include <algorithm>
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

namespace MathUtils {
    inline float vecLength(const sf::Vector2f &v) {
        return std::sqrt(v.x*v.x + v.y*v.y);
    }

    inline float vecLength(const sf::Vector3f &v) {
        return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    }

    inline float vecLengthSqr(const sf::Vector2f &v) {
        return v.x*v.x + v.y*v.y;
    }

    inline sf::Vector2f clamp(const sf::Vector2f &val, const sf::Vector2f &min, const sf::Vector2f &max) {
        return sf::Vector2f(std::clamp(val.x, min.x, max.x), std::clamp(val.y, min.y, max.y));
    }

    inline sf::Vector2f normalized(const sf::Vector2f &v) {
        float len = vecLength(v);
        if(len == 0) return v;
        return v / len;
    }

    inline sf::Vector3f normalized(const sf::Vector3f &v) {
        float len = vecLength(v);
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
