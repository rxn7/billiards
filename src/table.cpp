#include "table.h"
#include "ball.h"
#include "mathUtils.h"
#include <SFML/Graphics/RenderTarget.hpp>

Table::Table(sf::Vector2f size, sf::Color color) : m_Size(size), m_Color(color) {
    m_Shape.setFillColor(m_Color);
    m_Shape.setSize(m_Size);
    m_Shape.setOrigin(m_Size * 0.5f);
}

void Table::render(sf::RenderTarget &renderTarget) const {
    renderTarget.draw(m_Shape);
}

std::pair<bool, sf::Vector2f> Table::isBallOverlapping(const Ball &ball) const {
    bool overlapping = false;
    sf::Vector2f normal = {0.0f, 0.0f};

    if(ball.m_Position.x - Ball::RADIUS < -m_Size.x * 0.5f) {
        overlapping = true;
        normal += sf::Vector2f(1.0f, 0.0f);
    } else if(ball.m_Position.x + Ball::RADIUS > m_Size.x * 0.5f) {
        overlapping = true;
        normal += sf::Vector2f(-1.0f, 0.0f);
    }
    if(ball.m_Position.y - Ball::RADIUS < -m_Size.y * 0.5f) {
        overlapping = true;
        normal += sf::Vector2f(0.0f, 1.0f);
    } else if(ball.m_Position.y + Ball::RADIUS > m_Size.y * 0.5f) {
        overlapping = true;
        normal += sf::Vector2f(0.0f, -1.0f);
    }

    normal = MathUtils::normalized(normal);

    return std::make_pair(overlapping, normal);
}
