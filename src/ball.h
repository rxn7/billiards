#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Ball {
public:
    Ball(uint8_t number);

public:
    void render(sf::RenderWindow &window) const;

public:
    inline bool isSolid() const { return m_Number < 9; }
    inline uint8_t getNumber() const { return m_Number; }
    sf::Vector2f m_Velocity;

private:
    sf::CircleShape m_Shape;
    sf::Color m_Color;
    sf::Vector2f m_Position;
    float m_Radius;
    uint8_t m_Number;
};
