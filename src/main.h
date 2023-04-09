#pragma once

#include "SDL_events.h"
#include <glm/ext/vector_float2.hpp>
#include <iostream>
#include <memory>

constexpr float BASE_WINDOW_WIDTH = 1280.0f;
constexpr float BASE_WINDOW_HEIGHT = 720.0f;

void init();
void cleanup();
void handleEvent(const SDL_Event *event);
void resize(int width, int height);
void cueHitCueBall(const glm::vec2 &direction, const float force);
void setupBalls();
