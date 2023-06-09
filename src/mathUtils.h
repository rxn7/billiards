#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <SFML/Graphics/Glsl.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

namespace MathUtils {
	constexpr float BYTE_TO_FLOAT_COLOR = 1.0f / 255.0f;

	inline float lengthSqr(const sf::Vector2f &v) {
		return v.x * v.x + v.y * v.y;
	}

	inline float lengthSqr(const sf::Vector3f &v) {
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}

	template <typename T> inline float length(const T &v) {
		return std::sqrt(lengthSqr(v));
	}

	template <typename T> T normalized(const T &v) {
		float len = length(v);
		if (len == 0)
			return v;
		return v / len;
	}

	inline sf::Vector3f cross(const sf::Vector3f &a, const sf::Vector3f &b) {
		return sf::Vector3f(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}

	inline sf::Glsl::Vec3 colorToGlslVec3(const sf::Color &color) {
		return sf::Glsl::Vec3(color.r * BYTE_TO_FLOAT_COLOR, color.g * BYTE_TO_FLOAT_COLOR, color.b * BYTE_TO_FLOAT_COLOR);
	}
} // namespace MathUtils
