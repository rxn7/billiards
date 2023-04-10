#include <SFML/System/Vector2.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/System/Time.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <iostream>
#include <memory>
#include <algorithm>
#include <cmath>

#include "audio.h"
#include "pocket.h"
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
static sf::Time deltaTime;

static bool renderDebugPockets = false;
static bool renderDebugBalls = false;
static bool debugFollowCueBall = false;

int main(int argc, const char **argv) {
    init();

    sf::Clock frameClock;
    sf::Event event;
    RenderStats renderStats;
    while(window.isOpen()) {
        deltaTime = frameClock.restart();

        while(window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            handleEvent(event);
        }

        update();

        window.clear();

        render(renderStats);
        imgui(renderStats);
        ImGui::SFML::Render(window);

        window.display();
    }

    shutdown();
}

void init() {
    if(!sf::Shader::isAvailable()) {
        std::cerr << "ERROR: Your graphics card doesn't support GLSL shaders.\n";
        std::exit(EXIT_FAILURE);
        return;
    }

    Random::init();
    Audio::init();
    Pocket::init(table);
    Ball::init();

    window.create(sf::VideoMode(BASE_WINDOW_HEIGHT, BASE_WINDOW_HEIGHT), "Billiard by rxn7", sf::Style::Default);
    window.setVerticalSyncEnabled(true);

    ImGui::SFML::Init(window);

    view.setSize(BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT);
    view.setCenter(0.0f, 0.0f);
    resize(BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT);

    for(int i=0; i<=15; ++i) 
        balls.emplace_back(i);

    rackBalls();

    cue = std::make_unique<Cue>(window, cueBall);
}

void shutdown() {
    ImGui::SFML::Shutdown();
}

void update() {
    float dt = deltaTime.asSeconds();

    ImGui::SFML::Update(window, deltaTime);
    Physics::update(balls, table);

    for(Ball &ball : balls)
        ball.update(dt);

    cue->update(dt);
}

void render(RenderStats &stats) {
    stats.frameTimeMs = deltaTime.asMilliseconds() * 0.001f;
    stats.fps = 1.0f / deltaTime.asSeconds();

    if(debugFollowCueBall)
        view.setCenter(cueBall.m_Position);

    window.setView(view);

    sf::Clock renderTimeClock;
    table.render(window);
    stats.tableRenderTime = renderTimeClock.restart();

    for(Ball &ball : balls)
        ball.render(window);
    stats.ballsRenderTime = renderTimeClock.restart();

    cue->render(window);
    stats.cueRenderTime = renderTimeClock.restart();

    if(renderDebugBalls)
        for(Ball &ball : balls)
            ball.renderDebug(window);

    if(renderDebugPockets)
        Pocket::renderDebug(window);

    stats.debugRenderTime = renderTimeClock.restart();
}

void imgui(const RenderStats &renderStats) {
    static bool vsync = true;

    ImGui::Begin("Debug");
    if(ImGui::TreeNode("Render stats")) {
        ImGui::Text("FPS: %i", renderStats.fps);
        ImGui::Text("Frame time: %.10f ms", renderStats.frameTimeMs);
        ImGui::Text("Balls render time: %.10f ms", renderStats.ballsRenderTime.asMicroseconds() * 0.001f);
        ImGui::Text("Table render time: %.10f ms", renderStats.tableRenderTime.asMicroseconds() * 0.001f);
        ImGui::Text("Cue render time: %.10f ms", renderStats.cueRenderTime.asMicroseconds() * 0.001f);
        ImGui::Text("Debug render time: %.10f ms", renderStats.debugRenderTime.asMicroseconds() * 0.001f);
        ImGui::TreePop();
    }

    if(ImGui::TreeNode("Options")) {
        if(ImGui::Checkbox("VSync", &vsync))
            window.setVerticalSyncEnabled(vsync);

        ImGui::Checkbox("Draw pockets", &renderDebugPockets);
        ImGui::Checkbox("Draw ball's velocity", &renderDebugBalls);
        if(ImGui::Checkbox("Camera follow cue ball", &debugFollowCueBall))
            if(!debugFollowCueBall)
                view.setCenter(0,0);

        ImGui::TreePop();
    }

    ImGui::End();
}

void rackBalls() {
    constexpr uint8_t numbers[15] = { 1, 14, 2, 15, 3, 13, 8, 6, 12, 7, 10, 4, 9, 11, 5};
    const sf::Vector2f position = sf::Vector2f(table.getSize().x * 0.25, 0.0f);

    for(Ball &ball : balls) {
        ball.m_Velocity = {0,0};
        ball.m_InPocket = false;
    }

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

void resize(const unsigned int width, const unsigned int height) {
    window.setSize({width, height});

    const float aspectRatio = static_cast<float>(height) / static_cast<float>(width);
    view.setSize(BASE_WINDOW_WIDTH, BASE_WINDOW_WIDTH * aspectRatio);
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
            const sf::Vector2f mousePositionWorld(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
            if(cueBall.isPointOverlapping(mousePositionWorld, Ball::RADIUS * 2))
                cue->startAiming();
            break;
        }

        case sf::Event::MouseButtonReleased:
            cue->hit();
            break;

        case sf::Event::KeyPressed:
            switch(event.key.code) {
                case sf::Keyboard::I:
                    view.zoom(1.1);
                    break;

                case sf::Keyboard::O:
                    view.zoom(0.9);
                    break;

                case sf::Keyboard::R:
                    rackBalls();
                    break;

                default: break;
            }
            break;

        default: break;
    }
}
