#pragma once

#include <iostream>
#include <memory>
#include <SFML/Window/Event.hpp>

static constexpr float BASE_WINDOW_WIDTH = 1280.0f;
static constexpr float BASE_WINDOW_HEIGHT = 720.0f;

void init();
void handleEvent(const sf::Event &event);
void resize(const unsigned int width, const unsigned int height);
void cueHitCueBall(const sf::Vector2f &direction, const float force);
void setupBalls();
