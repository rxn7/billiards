#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "ball.h"

class Cue {
public:
    Cue(const sf::RenderWindow &window, Ball &cueBall);
    void render(sf::RenderTarget &renderTarget) const;
    void update(float dt);
    void hit();

public:
    bool m_Visible = false;

private:
    const sf::RenderWindow &m_Window;
    Ball &m_CueBall;
    float m_Force;
    sf::Vector2f m_Direction;
    sf::Sprite m_Sprite;
    sf::Texture m_Texture;
};
