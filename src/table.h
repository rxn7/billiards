#pragma once

#include "main.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

namespace TableColors {
    const sf::Color TOURNAMENT_BLUE = {0, 98, 162};
}

struct Ball;

class Table {
public:
    Table(sf::Vector2f size = {DEFAULT_WIDTH, DEFAULT_HEIGHT}, sf::Color color = TableColors::TOURNAMENT_BLUE);
    void render(sf::RenderTarget &renderTarget) const;
    std::pair<bool, sf::Vector2f> isBallOverlapping(const Ball &ball) const;
    inline const sf::Vector2f &getSize() const { return m_Size; }

public:
    static constexpr float DEFAULT_WIDTH = BASE_WINDOW_WIDTH * 0.75f;
    static constexpr float DEFAULT_HEIGHT = DEFAULT_WIDTH * 0.5f;

private:
    sf::Vector2f m_Size;
    sf::Color m_Color;
    sf::RectangleShape m_Shape;
};
