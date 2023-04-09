#include "cue.h"
#include "audio.h"
#include "mathUtils.h"

Cue::Cue(const sf::RenderWindow &window, Ball &cueBall) : m_Window(window), m_CueBall(cueBall) {
    m_Texture.loadFromFile("assets/textures/cue.png");
    m_Sprite.setTexture(m_Texture, true);
    m_Sprite.setScale(8,5);
    m_Sprite.setOrigin(sf::Vector2f(m_Texture.getSize()));
}

void Cue::update(float dt) {
    if(m_HitAnimation) {
        const sf::Vector2f delta = m_CueBall.m_Position - m_Sprite.getPosition();
        const sf::Vector2f direction = MathUtils::normalized(delta);
        const float distance = MathUtils::length(delta);
        if(distance <= Ball::RADIUS) {
            m_CueBall.m_Velocity += m_Direction * m_Force * 10.0f;
            m_Visible = false;
            m_HitAnimation = false;
            Audio::play(m_Sound, Audio::AudioType::CUE_HIT, 100.0f, 1.0f);
        }
        m_Sprite.move(direction * 2000.0f * dt);
        return;
    }

    if(!m_Visible)
        return;

    const sf::Vector2i mousePosition = sf::Mouse::getPosition() - m_Window.getPosition();
    const sf::Vector2f mousePositionWorld = m_Window.mapPixelToCoords(mousePosition);
    const sf::Vector2f delta = m_CueBall.m_Position - mousePositionWorld;
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

void Cue::triggerHitAnimation() {
    m_HitAnimation = true;
}
