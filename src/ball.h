#pragma once

#include "table.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Ball {
public:
    Ball(uint8_t number);

public:
    void update(float dt);
    void render(sf::RenderTarget &renderTarget) const;
    static void applyPhysics(std::vector<Ball> &balls);

    inline bool isPointOverlapping(sf::Vector2f v) const { 
        sf::Vector2f d = m_Position - v;
        return (d.x*d.x + d.y*d.y) < Ball::RADIUS * Ball::RADIUS;
    }
    inline bool isStriped() const { return m_Number > 8; }
    inline uint8_t getNumber() const { return m_Number; }

    const sf::Color &getColor() const;

private:
    static void init();

public:
    static constexpr float RADIUS = 20.0f;
    static constexpr float DIAMETER = RADIUS * 2.0f;
    static constexpr float DRAG_COEFFICIENT = 0.3f;
    sf::Vector2f m_Velocity = {0,0};
    sf::Vector2f m_Position = {0,0};
    sf::Vector3f m_Rotation = {0,0,0};
    float m_Scale = 1.0f;

private:
    uint8_t m_Number = 8;
};
