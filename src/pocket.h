#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
class Ball;
class Table;

namespace Pocket {
	constexpr float RADIUS = 23.0f;
	constexpr float OFFSET = 8.0f;

	void init(const Table &table);
	void renderDebug(sf::RenderTarget &renderTarget);
	bool isBallInsideAny(const Ball &ball);
	bool isBallOverlappingAny(const Ball &ball);
} // namespace Pocket
