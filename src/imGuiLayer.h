#pragma once

#include "renderStats.h"
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

class Game;

class ImGuiLayer {
public:
    ImGuiLayer(Game &game);
    ~ImGuiLayer();

    void handleEvent(const sf::Event &event) const;
    void update(const sf::Time &frameTime) const;
    void render(const RenderStats &renderStats) const;

private:
    Game &m_Game;
};
