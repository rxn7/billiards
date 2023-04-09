#pragma once

#include <iostream>
#include <memory>
#include <SFML/Window/Event.hpp>

static constexpr float BASE_WINDOW_WIDTH = 1280.0f;
static constexpr float BASE_WINDOW_HEIGHT = 720.0f;

void init();
void shutdown();
void update(float dt);
void render();
void rackBalls();
void resize(const unsigned int width, const unsigned int height);
void handleEvent(const sf::Event &event);
