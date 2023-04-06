#include <SFML/System/Vector2.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <iostream>
#include <memory>
#include <algorithm>
#include <cmath>

#include "main.h"
#include "ball.h"
#include "mathUtils.h"
#include "table.h"

#define cueBall balls[0]

static constexpr float INIT_CUE_BALL_POSITION_NORMALIZED = 0.2f;
static std::unique_ptr<sf::RenderWindow> window;
static std::vector<Ball> balls;
static Table table;
static sf::View view;

static bool isCueBallHeld = false;
static sf::Vector2f cueBallHeldStartPosition;

int main(int argc, const char **argv) {
    init();

    sf::Clock frameClock;
    while(window->isOpen()) {
        float dt = frameClock.restart().asSeconds();

        sf::Event event;
        while(window->pollEvent(event))
            handleEvent(event);

        window->clear();

        Ball::applyPhysics(balls, table);

        table.render(*window);

        for(Ball &ball : balls) {
            ball.update(dt);
            ball.render(*window);
        }

        window->display();
    }
}

void cueHitCueBall(sf::Vector2f direction, float force) {
    cueBall.m_Velocity = direction * std::clamp(force, 5.0f, 3000.0f);
}

void resize(unsigned int width, unsigned int height) {
    static constexpr float baseRatio = BASE_WINDOW_WIDTH / BASE_WINDOW_HEIGHT;
    float windowRatio = static_cast<float>(width) / static_cast<float>(height);

    if(width >= baseRatio * height) {
        float size = static_cast<float>(height) * baseRatio;
        float offset = (width - size) * 0.5f;
        view.setViewport({offset / width, 0.0f, size / static_cast<float>(width), 1.0f});
    } else {
        float size = static_cast<float>(width) / baseRatio;
        float offset = (height - size) * 0.5f;
        view.setViewport({0.0f, offset / height, 1.0f, size / static_cast<float>(height)});
    }

    window->setSize({width, height});
    window->setView(view);
}

void init() {
    if(!sf::Shader::isAvailable()) {
        std::cerr << "ERROR: Your graphics card doesn't support GLSL shaders.\n";
        std::exit(EXIT_FAILURE);
        return;
    }

    for(int i=0; i<=15; ++i) balls.emplace_back(i);

    setupBalls();

    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(BASE_WINDOW_HEIGHT, BASE_WINDOW_HEIGHT), "Billard by rxn7", sf::Style::Default);
    window->setVerticalSyncEnabled(true);

    view.setSize(BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT);
    view.setCenter(0.0f, 0.0f);
    resize(BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT);
}

void setupBalls() {
    uint8_t numbers[15] = { 1, 14, 2, 15, 3, 13, 8, 6, 12, 7, 10, 4, 9, 11, 5};
    sf::Vector2f position = sf::Vector2f(table.getSize().x * 0.2, 0.0f);

    static const float width = sqrt(3.0f) * Ball::RADIUS;
    static const float height = Ball::DIAMETER;

    for(Ball &ball : balls) {
        ball.m_Velocity = {0,0};
    }

    int idx = 0;
    for(int row=-2; row<=2; ++row) {
        for(int col=row; col<=2; ++col) {
            balls[numbers[idx++]].m_Position = position + sf::Vector2f(
                col * width,
                row * height - col * Ball::RADIUS + Ball::DIAMETER
            );
        }
    }

    cueBall.m_Position = sf::Vector2f(-table.getSize().x * (1.0 - INIT_CUE_BALL_POSITION_NORMALIZED) * 0.5f, 0.0f);
}

void handleEvent(sf::Event &event) {
    switch(event.type) {
        case sf::Event::Closed:
            window->close();
            break;

        case sf::Event::Resized: {
            resize(event.size.width, event.size.height);
            break;
        }

        case sf::Event::MouseButtonPressed: {
            sf::Vector2 pos = window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            if(cueBall.isPointOverlapping(pos)) {
                isCueBallHeld = true;
                cueBallHeldStartPosition = pos;
            }
            break;
        }

        case sf::Event::MouseButtonReleased:
            if(isCueBallHeld) {
                isCueBallHeld = false;
                sf::Vector2f delta = cueBallHeldStartPosition - window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                float force = MathUtils::vecLength(delta);
                sf::Vector2f direction = MathUtils::normalized(delta);
                cueHitCueBall(direction, force);
            }
            break;

        case sf::Event::KeyPressed:
            switch(event.key.code) {
                case sf::Keyboard::I:
                    view.zoom(1.1);
                    window->setView(view);
                    break;

                case sf::Keyboard::O:
                    view.zoom(0.9);
                    window->setView(view);
                    break;

                case sf::Keyboard::R:
                    setupBalls();
                    break;

                default: break;
            }
            break;

        default: break;
    }
}