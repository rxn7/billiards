#include "game.h"
#include "physics.h"
#include "random.h"
#include "audio.h"
#include "pocket.h"

#include <SFML/Window/WindowStyle.hpp>

#include <iostream>
#include <pthread.h>

Game::Game() : m_ImGuiLayer(*this) {
    if(!sf::Shader::isAvailable()) {
        std::cerr << "ERROR: Your graphics card doesn't support GLSL shaders.\n";
        std::exit(EXIT_FAILURE);
        return;
    }

    m_Window.create(sf::VideoMode(WindowProperties::WINDOW_BASE_WIDTH, WindowProperties::WINDOW_BASE_HEIGHT), "Billiards by rxn7", sf::Style::Default);
    m_Window.setVerticalSyncEnabled(true);

    Random::init();
    Audio::init();
    Ball::init();
    Pocket::init(m_Table);

    m_View.setCenter(0.0f, 0.0f);
    resize(WindowProperties::WINDOW_BASE_WIDTH, WindowProperties::WINDOW_BASE_HEIGHT);

    m_Balls.reserve(16);
    for(int i=0; i<16; ++i)
        m_Balls.emplace_back(i);

    mp_CueBall = &m_Balls.at(0);
    mp_Cue = std::make_unique<Cue>(m_Window, mp_CueBall);

    rackBalls();
}

Game::~Game() {
}

void Game::start() {
    sf::Clock frameClock;
    while(m_Window.isOpen()) {
        m_FrameTime = frameClock.restart();

        sf::Event event;
        while(m_Window.pollEvent(event)) {
            m_ImGuiLayer.handleEvent(event);
            handleEvent(event);
        }

        update();

        m_Window.clear();

        render();

        m_Window.display();
    }
}

void Game::update() {
    float dt = m_FrameTime.asSeconds();

    sf::Clock physicsUpdateTimeClock;
    Physics::update(m_Balls, m_Table);
    m_PerfStats.physicsUpdateTime = physicsUpdateTimeClock.getElapsedTime();

    for(Ball &ball : m_Balls)
        ball.update(dt);
    mp_Cue->update(dt);

    m_ImGuiLayer.update(m_FrameTime);
}

void Game::render() {
    m_PerfStats.frameTimeMs = m_FrameTime.asMicroseconds() * 0.001f;
    m_PerfStats.fps = 1.0f / m_FrameTime.asSeconds();

    if(m_Options.cameraFollowCueBall)
        m_View.setCenter(mp_CueBall->m_Position);

    m_Window.setView(m_View);

    m_Table.render(m_Window);

    sf::Clock ballsRenderTimeClock;
    Ball::s_Shader.setUniform("u_LightPosition", m_LightProps.lightPosition);
    Ball::s_Shader.setUniform("u_LightColor", m_LightProps.lightColor);
    Ball::s_Shader.setUniform("u_AmbientIntensity", m_LightProps.ambientIntensity);
    Ball::s_Shader.setUniform("u_DiffuseIntensity", m_LightProps.diffuseIntensity);
    Ball::s_Shader.setUniform("u_SpecularIntensity", m_LightProps.specularIntensity);
    Ball::s_Shader.setUniform("u_Shininess", m_LightProps.shininess);
    for(const Ball &ball : m_Balls)
        ball.render(m_Window);
    m_PerfStats.ballsRenderTime = ballsRenderTimeClock.getElapsedTime();

    mp_Cue->render(m_Window);

    if(m_Options.renderBallVelocity)
        for(const Ball &ball : m_Balls)
            ball.renderDebug(m_Window);

    sf::Clock debugRenderTimeClock;
    if(m_Options.renderPocket)
        Pocket::renderDebug(m_Window);
    m_PerfStats.debugRenderTime = debugRenderTimeClock.getElapsedTime();

    m_ImGuiLayer.render(m_PerfStats);
}

void Game::rackBalls() {
    constexpr uint8_t numbers[15] = { 1, 14, 2, 15, 3, 13, 8, 6, 12, 7, 10, 4, 9, 11, 5};
    const sf::Vector2f position = sf::Vector2f(m_Table.getSize().x * 0.25, 0.0f);

    for(Ball &ball : m_Balls) {
        ball.m_Velocity = {0,0};
        ball.m_InPocket = false;
    }

    static const float width = std::sqrt(3.0f) * Ball::RADIUS + Physics::COLLISION_MARGIN;
    static constexpr float height = Ball::DIAMETER + Physics::COLLISION_MARGIN;

    int idx = 0;
    for(int row=0; row<5; ++row) {
        for(int col=row; col<5; ++col) {
            m_Balls[numbers[idx++]].m_Position = position + sf::Vector2f(
                col * width,
                row * height - col * Ball::RADIUS
            );
        }
    }

    mp_CueBall->m_Position = sf::Vector2f(-m_Table.getSize().x * 0.4f, 0.0f);
}

void Game::resize(const unsigned int width, const unsigned int height) {
    m_Window.setSize({width, height});

    const float aspectRatio = static_cast<float>(height) / static_cast<float>(width);
    m_View.setSize(WindowProperties::WINDOW_BASE_WIDTH, WindowProperties::WINDOW_BASE_WIDTH * aspectRatio);
}

void Game::handleEvent(const sf::Event &event) {
    switch(event.type) {
        case sf::Event::Closed:
            m_Window.close();
            break;

        case sf::Event::Resized: {
            resize(event.size.width, event.size.height);
            break;
        }

        case sf::Event::MouseButtonPressed: {
            const sf::Vector2f mousePositionWorld(m_Window.mapPixelToCoords(sf::Mouse::getPosition(m_Window)));
            if(mp_CueBall->isPointOverlapping(mousePositionWorld, Ball::RADIUS * 2))
                mp_Cue->startAiming();
            break;
        }

        case sf::Event::MouseButtonReleased:
            mp_Cue->hit();
            break;

        case sf::Event::KeyPressed:
            switch(event.key.code) {
                case sf::Keyboard::I:
                    m_View.zoom(1.1);
                    break;

                case sf::Keyboard::O:
                    m_View.zoom(0.9);
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
