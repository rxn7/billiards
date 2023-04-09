#pragma once

#include "table.h"
#include <SDL2/SDL.h>
#include <glm/gtc/quaternion.hpp>

class Ball {
public:
    Ball(const uint8_t number);

public:
    void update(const float dt);
    void render() const;

    inline bool isPointOverlapping(const glm::vec2 &v) const { 
        const glm::vec2 d = m_Position - v;
        return (d.x*d.x + d.y*d.y) < Ball::RADIUS * Ball::RADIUS;
    }

    static const SDL_Color &getColor(const int number);
    inline bool isStriped() const { return m_Number > 8; }
    inline uint8_t getNumber() const { return m_Number; }

private:
    static void init();
    void applyDrag(const float speed, const float dt);
    void applyRotation(const float speed, const glm::vec2 &movement, const float dt);

public:
    static constexpr float RADIUS = 20.0f;
    static constexpr float DIAMETER = RADIUS * 2.0f;
    static constexpr float DRAG_COEFFICIENT = 0.3f;
    static constexpr float MASS = 0.160f;
    glm::vec2 m_Velocity = {0,0};
    glm::vec2 m_Position = {0,0};
    float m_Scale = 1.0f;
    // sf::Sound m_Sound;

private:
    glm::quat m_Rotation;
    const SDL_Color &m_Color;
    uint8_t m_Number = 8;
};
