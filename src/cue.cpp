#include "cue.h"
#include "mathUtils.h"

Cue::Cue(const sf::RenderWindow &window, Ball &cueBall) : m_Window(window), m_CueBall(cueBall) {
    m_Texture.loadFromFile("res/textures/cue.png");
    m_Sprite.setTexture(m_Texture, true);
    m_Sprite.setScale(8,5);
    m_Sprite.setOrigin(sf::Vector2f(m_Texture.getSize()));
}

void Cue::update(float dt) {
    const sf::Vector2f mousePosition = m_Window.mapPixelToCoords(sf::Mouse::getPosition());
    const sf::Vector2f delta = m_CueBall.m_Position - mousePosition;
    m_Direction = MathUtils::normalized(delta);

    m_Force = std::clamp(MathUtils::length(delta), 0.0f, 100.0f);
    const sf::Vector2 position = m_CueBall.m_Position - m_Direction * m_Force;

    m_Sprite.setPosition(position);
    m_Sprite.setRotation(std::atan2(delta.y, delta.x) * 180.0f / M_PIf);
}

void Cue::render(sf::RenderTarget &renderTarget) const {
    if(!m_Visible)
        return;

    renderTarget.draw(m_Sprite);
}

void Cue::hit() {
    m_CueBall.m_Velocity += m_Direction * m_Force * 10.0f;
    m_Visible = false;
}
