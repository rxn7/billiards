#pragma once

#include <iostream>
#include <memory>
#include <SFML/Window/Event.hpp>

static constexpr float BASE_WINDOW_WIDTH = 1280.0f;
static constexpr float BASE_WINDOW_HEIGHT = 720.0f;

void init();
void handleEvent(sf::Event &event);
void resize(unsigned int width, unsigned int height);
void setupBalls();