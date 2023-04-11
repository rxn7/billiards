#pragma once

#include <cinttypes>
#include <SFML/System/Time.hpp>

struct PerformanceStats {
    float frameTimeMs;
    std::uint32_t fps;
    sf::Time physicsUpdateTime;
    sf::Time ballsRenderTime;
    sf::Time debugRenderTime;
};
