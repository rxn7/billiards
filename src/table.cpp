#include "table.h"

Table::Table(sf::Vector2f size, sf::Color color) : m_Size(size), m_Color(color) {
    m_Shape.setFillColor(m_Color);
    m_Shape.setSize(m_Size);
    m_Shape.setOrigin(m_Size * 0.5f);
}

void Table::render(sf::RenderWindow &window) const {
    window.draw(m_Shape);
}
