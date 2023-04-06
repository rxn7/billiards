#include "table.h"
#include <SFML/Graphics/RenderTarget.hpp>

Table::Table(sf::Vector2f size, sf::Color color) : m_Size(size), m_Color(color) {
    m_Shape.setFillColor(m_Color);
    m_Shape.setSize(m_Size);
    m_Shape.setOrigin(m_Size * 0.5f);
}

void Table::render(sf::RenderTarget &renderTarget) const {
    renderTarget.draw(m_Shape);
}
