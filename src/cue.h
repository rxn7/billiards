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
    void hitAnimationStep(float dt);
    void startAiming();
    void hit();

public:
    bool m_Aiming = false;

private:
    static constexpr float HIT_ANIMATION_SPEED = 2000.0f;
    const sf::RenderWindow &m_Window;
    Ball &m_CueBall;
    float m_Force;
    bool m_HitAnimation = false;
    sf::Vector2f m_Direction;
    sf::Sprite m_Sprite;
    sf::Sound m_Sound;
    sf::Texture m_Texture;
};
