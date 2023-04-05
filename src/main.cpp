#include <SFML/Window/Event.hpp>
#include <iostream>
#include <memory>

#include "main.h"
#include "ball.h"
#include "table.h"

static std::unique_ptr<sf::RenderWindow> window;
static std::vector<Ball> balls;
static Table table;
static sf::View view;

int main(int argc, const char **argv) {
    init();

    while(window->isOpen()) {
        sf::Event event;
        while(window->pollEvent(event))
            handleEvent(event);

        window->clear();
        table.Draw(*window);
        window->display();
    }
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

        default: break;
    }
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
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(BASE_WINDOW_HEIGHT, BASE_WINDOW_HEIGHT), "Billard by rxn7");
    window->setVerticalSyncEnabled(true);

    view.setSize(BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT);
    view.setCenter(0.0f, 0.0f);
    resize(BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT);
}