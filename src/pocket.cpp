#include "pocket.h"
#include "ball.h"
#include "table.h"
#include "mathUtils.h"
#include <SFML/System/Vector2.hpp>
#include <array>

static std::array<sf::Vector2f, 6> pockets;
static sf::CircleShape shape;

void Pocket::init(const Table &table) {
    const sf::Vector2f &tableSize = table.getSize();

    const float hw = tableSize.x * 0.5f - OFFSET - RADIUS;
    const float hh = tableSize.y * 0.5f - OFFSET - RADIUS;

    pockets = {
        sf::Vector2f(-hw + OFFSET, hh - OFFSET), // Top left
        sf::Vector2f(hw - OFFSET, hh - OFFSET), // Top right
        sf::Vector2f(-hw + OFFSET, -hh + OFFSET), // Bottom left
        sf::Vector2f(hw - OFFSET, -hh + OFFSET), // Bottom right
        sf::Vector2f(0, hh), // Middle top
        sf::Vector2f(0, -hh) // Middle bottom
    };

    shape.setRadius(RADIUS);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(RADIUS, RADIUS);
}

bool Pocket::isBallInsideAny(const Ball &ball) {
    for(const sf::Vector2f &pocket : pockets) {
        const float distanceSqr = MathUtils::lengthSqr(ball.m_Position - pocket);
        if(distanceSqr <= RADIUS * RADIUS)
            return true;
    }

    return false;
}

void Pocket::renderDebug(sf::RenderTarget &renderTarget) {
    for(const sf::Vector2f &pocket : pockets) {
        shape.setPosition(pocket);
        renderTarget.draw(shape);
    }
}
