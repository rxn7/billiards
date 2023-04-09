#include <SFML/System/Vector2.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <iostream>
#include <memory>
#include <algorithm>
#include <cmath>

#include "audio.h"
#include "random.h"
#include "main.h"
#include "ball.h"
#include "mathUtils.h"
#include "table.h"
#include "cue.h"
#include "physics.h"

#define cueBall balls[0]

static constexpr float INIT_CUE_BALL_POSITION_NORMALIZED = 0.2f;
static sf::RenderWindow window;
static std::vector<Ball> balls;
static Table table;
static std::unique_ptr<Cue> cue;
static sf::View view;

int main(int argc, const char **argv) {
    init();

    sf::Clock frameClock;
    while(window.isOpen()) {
        float dt = frameClock.restart().asSeconds();

        sf::Event event;
        while(window.pollEvent(event))
            handleEvent(event);

        window.clear();

        Physics::update(balls, table);

        table.render(window);

        for(Ball &ball : balls) {
            ball.update(dt);
            ball.render(window);
        }

        cue->update(dt);
        cue->render(window);

        window.display();
    }
}

void resize(const unsigned int width, const unsigned int height) {
    static constexpr float baseRatio = BASE_WINDOW_WIDTH / BASE_WINDOW_HEIGHT;
    const float windowRatio = static_cast<float>(width) / static_cast<float>(height);

    if(width >= baseRatio * height) {
        const float size = static_cast<float>(height) * baseRatio;
        const float offset = (width - size) * 0.5f;
        view.setViewport({offset / width, 0.0f, size / static_cast<float>(width), 1.0f});
    } else {
        const float size = static_cast<float>(width) / baseRatio;
        const float offset = (height - size) * 0.5f;
        view.setViewport({0.0f, offset / height, 1.0f, size / static_cast<float>(height)});
    }

    window.setSize({width, height});
    window.setView(view);
}

void init() {
    if(!sf::Shader::isAvailable()) {
        std::cerr << "ERROR: Your graphics card doesn't support GLSL shaders.\n";
        std::exit(EXIT_FAILURE);
        return;
    }

    Audio::init();
    Random::init();

    for(int i=0; i<=15; ++i) balls.emplace_back(i);

    setupBalls();

    window.create(sf::VideoMode(BASE_WINDOW_HEIGHT, BASE_WINDOW_HEIGHT), "Billiard by rxn7", sf::Style::Default);
    window.setVerticalSyncEnabled(true);

    cue = std::make_unique<Cue>(window, cueBall);

    view.setSize(BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT);
    view.setCenter(0.0f, 0.0f);
    resize(BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT);
}

void setupBalls() {
    constexpr uint8_t numbers[15] = { 1, 14, 2, 15, 3, 13, 8, 6, 12, 7, 10, 4, 9, 11, 5};
    const sf::Vector2f position = sf::Vector2f(table.getSize().x * 0.25, 0.0f);

    for(Ball &ball : balls)
        ball.m_Velocity = {0,0};

    static constexpr float width = std::sqrt(3.0f) * Ball::RADIUS + Physics::COLLISION_MARGIN;
    static constexpr float height = Ball::DIAMETER + Physics::COLLISION_MARGIN;

    int idx = 0;
    for(int row=0; row<5; ++row) {
        for(int col=row; col<5; ++col) {
            balls[numbers[idx++]].m_Position = position + sf::Vector2f(
                col * width,
                row * height - col * Ball::RADIUS
            );
        }
    }

    cueBall.m_Position = sf::Vector2f(-table.getSize().x * (1.0 - INIT_CUE_BALL_POSITION_NORMALIZED) * 0.5f, 0.0f);
}

void handleEvent(const sf::Event &event) {
    switch(event.type) {
        case sf::Event::Closed:
            window.close();
            break;

        case sf::Event::Resized: {
            resize(event.size.width, event.size.height);
            break;
        }

        case sf::Event::MouseButtonPressed: {
            cue->m_Visible = true;
            break;
        }

        case sf::Event::MouseButtonReleased:
            cue->triggerHitAnimation();
            break;

        case sf::Event::KeyPressed:
            switch(event.key.code) {
                case sf::Keyboard::I:
                    view.zoom(1.1);
                    window.setView(view);
                    break;

                case sf::Keyboard::O:
                    view.zoom(0.9);
                    window.setView(view);
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
