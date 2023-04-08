#pragma once

#include "table.h"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Glsl.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <glm/gtc/quaternion.hpp>

class Ball {
public:
    Ball(const uint8_t number);

public:
    void update(const float dt);
    void render(sf::RenderTarget &renderTarget) const;

    inline bool isPointOverlapping(const sf::Vector2f &v) const { 
        sf::Vector2f d = m_Position - v;
        return (d.x*d.x + d.y*d.y) < Ball::RADIUS * Ball::RADIUS;
    }

    static const sf::Color &getColor(const int number);
    inline bool isStriped() const { return m_Number > 8; }
    inline uint8_t getNumber() const { return m_Number; }

private:
    static void init();
    void applyDrag(const float speed, const float dt);
    void applyRotation(const float speed, const sf::Vector2f &movement, const float dt);

public:
    static constexpr float RADIUS = 20.0f;
    static constexpr float DIAMETER = RADIUS * 2.0f;
    static constexpr float DRAG_COEFFICIENT = 0.3f;
    static constexpr float MASS = 0.160f;
    sf::Vector2f m_Velocity = {0,0};
    sf::Vector2f m_Position = {0,0};
    float m_Scale = 1.0f;
    sf::Sound m_Sound;

private:
    glm::quat m_Rotation;
    const sf::Color &m_Color;
    uint8_t m_Number = 8;
};
