#include "cue.h"
#include "audio.h"
#include "mathUtils.h"

Cue::Cue(const sf::RenderWindow &window, Ball &cueBall) : m_Window(window), m_CueBall(cueBall) {
    m_Texture.loadFromFile("assets/textures/cue.png");
    const sf::Vector2f textureSize(m_Texture.getSize());

    m_Sprite.setTexture(m_Texture, true);
    m_Sprite.setScale(8,5);

    m_Sprite.setOrigin(textureSize.x, textureSize.y * 0.5f);
}

void Cue::update(float dt) {
    if(m_CueBall.m_InPocket)
        m_Aiming = false;

    if(!m_Aiming)
        return;

    if(m_HitAnimation) {
        hitAnimationStep(dt);
        return;
    }

    const sf::Vector2f mousePositionWorld = m_Window.mapPixelToCoords(sf::Mouse::getPosition(m_Window));
    const sf::Vector2f delta = m_CueBall.m_Position - mousePositionWorld;
    m_Direction = MathUtils::normalized(delta);

    const float forcePercentage = std::clamp(MathUtils::length(delta) / VISUAL_RANGE, 0.0f, 1.0f);
    m_Force = std::lerp(MIN_FORCE, MAX_FORCE, forcePercentage);

    m_Sprite.setPosition(m_CueBall.m_Position - m_Direction * forcePercentage * VISUAL_RANGE);
    m_Sprite.setRotation(glm::degrees(std::atan2(delta.y, delta.x)));
}

void Cue::render(sf::RenderTarget &renderTarget) const {
    if(!m_Aiming)
        return;

    renderTarget.draw(m_Sprite);
}

void Cue::hitAnimationStep(float dt) {
    const sf::Vector2f delta = m_CueBall.m_Position - m_Sprite.getPosition();
    const sf::Vector2f direction = MathUtils::normalized(delta);
    const float distance = MathUtils::length(delta);

    m_Sprite.move(direction * HIT_ANIMATION_SPEED * dt);

    if(distance <= Ball::RADIUS)
        actualHit();
}

void Cue::actualHit() {
    m_Aiming = false;
    m_HitAnimation = false;

    m_CueBall.m_Velocity += m_Direction * m_Force;

    const float volume = m_Force / MAX_FORCE * 100.0f;
    const float pitch = (1.0f + m_Force / MAX_FORCE) * 0.5f;

    Audio::play(m_Sound, Audio::AudioType::CUE_HIT, volume, pitch);
}

void Cue::startAiming() {
    if(m_CueBall.m_InPocket)
        return;

    m_Aiming = true;
}

void Cue::hit() {
    if(!m_Aiming || m_CueBall.m_InPocket)
        return;

    m_HitAnimation = true;
}
