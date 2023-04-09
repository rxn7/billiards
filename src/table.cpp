#include "table.h"
#include "ball.h"
#include "mathUtils.h"
#include <SFML/Graphics/RenderTarget.hpp>

Table::Table(const sf::Vector2f &size, const sf::Color &color) : m_Size(size), m_Color(color) {
    assert(m_Texture.loadFromFile("res/textures/table.png"));

    const sf::Vector2f textureSize(m_Texture.getSize());
    float scale = size.x / textureSize.x;

    m_Sprite.setTexture(m_Texture, true);
    m_Sprite.setScale(scale, scale);
    m_Sprite.setPosition(-size * 0.5f);
}

void Table::render(sf::RenderTarget &renderTarget) const {
    renderTarget.draw(m_Sprite);
}

std::pair<bool, sf::Vector2f> Table::isBallOverlapping(const Ball &ball) const {
    bool overlapping = false;
    sf::Vector2f normal = {0.0f, 0.0f};

    const float margin = getMargin();

    if(ball.m_Position.x - Ball::RADIUS < -m_Size.x * 0.5f + margin) {
        overlapping = true;
        normal += sf::Vector2f(1.0f, 0.0f);
    } else if(ball.m_Position.x + Ball::RADIUS > m_Size.x * 0.5f - margin) {
        overlapping = true;
        normal += sf::Vector2f(-1.0f, 0.0f);
    }
    if(ball.m_Position.y - Ball::RADIUS < -m_Size.y * 0.5f + margin) {
        overlapping = true;
        normal += sf::Vector2f(0.0f, 1.0f);
    } else if(ball.m_Position.y + Ball::RADIUS > m_Size.y * 0.5f - margin) {
        overlapping = true;
        normal += sf::Vector2f(0.0f, -1.0f);
    }

    normal = MathUtils::normalized(normal);

    return std::make_pair(overlapping, normal);
}
