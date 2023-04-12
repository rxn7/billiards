#pragma once

#include "performanceStats.h"
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#ifndef DEBUG
#define NO_IMGUI
#endif

class Game;

class ImGuiLayer {
public:
    ImGuiLayer(Game &game);
    ~ImGuiLayer();

    void handleEvent(const sf::Event &event) const;
    void update(const sf::Time &frameTime) const;
    void render(const PerformanceStats &performanceStats) const;

private:
    Game &m_Game;
};
