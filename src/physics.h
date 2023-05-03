#pragma once

#include "ball.h"
#include "table.h"

namespace Physics {
	constexpr float COLLISION_MARGIN = 0.001f;
	constexpr std::uint32_t MIN_STEPS_PER_SEC = 0u;
	constexpr std::uint32_t MAX_STEPS_PER_SEC = 100000u;

	void init(std::vector<Ball> *balls);
	void step(float ts);
	void update(float dt);
	void resolveCollision(Ball &a, Ball &b);
	void resolveTableCollision(Ball &ball, const Table::OverlapResult &result, const Table &table);
	bool isInsidePocket(const Ball &ball);

	inline std::uint32_t stepsPerSecond = 1000u;
}; // namespace Physics
