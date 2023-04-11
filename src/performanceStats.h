#pragma once

#include <cinttypes>
#include <SFML/System/Time.hpp>

struct PerformanceStats {
    float frameTimeMs;
    std::uint32_t fps;
    sf::Time ballsRenderTime;
    sf::Time tableRenderTime;
    sf::Time debugRenderTime;
    sf::Time cueRenderTime;
};
