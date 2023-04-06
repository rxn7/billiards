#pragma once

#include "table.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Ball {
public:
    Ball(uint8_t number);

public:
    void render(sf::RenderTarget &renderTarget) const;
    inline bool isStriped() const { return m_Number > 8; }
    inline uint8_t getNumber() const { return m_Number; }
    const sf::Color &getColor() const;

private:
    static void init();

public:
    static constexpr float RADIUS = 20.0f;
    static constexpr float DIAMETER = RADIUS * 2.0f;
    sf::Vector2f m_Velocity = {0,0};
    sf::Vector2f m_Position = {0,0};
    float m_Scale = 1.0f;
    float m_Angle = 0.0f;

private:
    uint8_t m_Number = 8;
};
