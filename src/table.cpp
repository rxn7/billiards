#include "table.h"
#include "SDL_render.h"
#include "ball.h"
#include <glm/ext/vector_float2.hpp>

Table::Table(const glm::vec2 &size, const SDL_Color &color) : m_Size(size), m_Color(color) {
    m_Rect.x = -size.x * 0.5f;
    m_Rect.y = -size.y * 0.5f;
    m_Rect.w = size.x;
    m_Rect.h = size.y;
}

void Table::render(SDL_Renderer *renderer, const glm::vec2 &camera) const {
    SDL_FRect drawingRect = {m_Rect.x - camera.x, m_Rect.y - camera.y, m_Rect.w, m_Rect.h};

    SDL_SetRenderDrawColor(renderer, m_Color.r, m_Color.g, m_Color.b, 255);
    SDL_RenderFillRectF(renderer, &drawingRect);
}

std::pair<bool, glm::vec2> Table::isBallOverlapping(const Ball &ball) const {
    bool overlapping = false;
    glm::vec2 normal(0.0f, 0.0f);

    if(ball.m_Position.x - Ball::RADIUS < -m_Size.x * 0.5f) {
        overlapping = true;
        normal += glm::vec2(1.0f, 0.0f);
    } else if(ball.m_Position.x + Ball::RADIUS > m_Size.x * 0.5f) {
        overlapping = true;
        normal += glm::vec2(-1.0f, 0.0f);
    }
    if(ball.m_Position.y - Ball::RADIUS < -m_Size.y * 0.5f) {
        overlapping = true;
        normal += glm::vec2(0.0f, 1.0f);
    } else if(ball.m_Position.y + Ball::RADIUS > m_Size.y * 0.5f) {
        overlapping = true;
        normal += glm::vec2(0.0f, -1.0f);
    }

    return std::make_pair(overlapping, glm::normalize(normal));
}
