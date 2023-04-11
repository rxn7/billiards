#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>

#include "imGuiLayer.h"
#include "ball.h"
#include "cue.h"
#include "gameOptions.h"
#include "lightingProperties.h"
#include "gameOptions.h"
#include "renderStats.h"

class Game {
public:
    Game();
    ~Game();

    void start();

private:
    void update();
    void render(RenderStats &stats);
    void rackBalls();
    void resize(const unsigned int width, const unsigned int height);
    void handleEvent(const sf::Event &event);

public:
    sf::RenderWindow m_Window;

private:
    friend ImGuiLayer;
    GameOptions m_Options;

    friend ImGuiLayer;
    LightingProperties m_LightProps;

    Ball *mp_CueBall;
    Table m_Table;
    std::vector<Ball> m_Balls;
    std::unique_ptr<Cue> mp_Cue;

    ImGuiLayer m_ImGuiLayer;

    sf::View m_View;
    sf::Time m_FrameTime;
};
