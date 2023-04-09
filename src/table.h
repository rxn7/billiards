#pragma once

#include "SDL_render.h"
#include "main.h"

namespace TableColors {
    const SDL_Color TOURNAMENT_BLUE = {0, 98, 162};
}

struct Ball;

class Table {
public:
    Table(const glm::vec2 &size = {DEFAULT_WIDTH, DEFAULT_HEIGHT}, const SDL_Color &color = TableColors::TOURNAMENT_BLUE);

    void render(SDL_Renderer *renderer, const glm::vec2 &camera) const;
    std::pair<bool, glm::vec2> isBallOverlapping(const Ball &ball) const;

    inline const glm::vec2 &getSize() const { return m_Size; }

public:
    static constexpr float DEFAULT_WIDTH = BASE_WINDOW_WIDTH * 0.75f;
    static constexpr float DEFAULT_HEIGHT = DEFAULT_WIDTH * 0.5f;

private:
    glm::vec2 m_Size;
    SDL_Color m_Color;
    SDL_FRect m_Rect;
};
