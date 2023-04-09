#include <iostream>
#include <memory>
#include <algorithm>
#include <cmath>
#include <vector>

#include "SDL.h"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_mouse.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "audio.h"
#include "random.h"
#include "main.h"
#include "ball.h"
#include "table.h"
#include "physics.h"

#define cueBall balls[0]

static constexpr float INIT_CUE_BALL_POSITION_NORMALIZED = 0.2f;

static std::vector<Ball> balls;
static Table table;

static SDL_Window *window;
static SDL_Renderer *renderer;
static glm::vec2 camera;
static bool quit = false;

static bool isCueBallHeld = false;
static glm::vec2 cueBallHeldStartPosition;

int main(int argc, const char **argv) {
    init();

    std::uint64_t now = SDL_GetPerformanceCounter();
    std::uint64_t last = 0;
    while(!quit) {
        now = SDL_GetPerformanceCounter();
        float dt = (now - last) / static_cast<float>(SDL_GetPerformanceFrequency());
        last = now;

        SDL_Event event;
        while(SDL_PollEvent(&event))
            handleEvent(&event);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        Physics::update(balls, table);

        table.render(renderer, camera);

        for(Ball &ball : balls) {
            ball.update(dt);
            ball.render();
        }

        SDL_RenderPresent(renderer);
    }

    cleanup();
}

void cueHitCueBall(const glm::vec2 &direction, const float force) {
    // TODO: Play sound
    cueBall.m_Velocity = direction * std::clamp(force, 5.0f, 3000.0f);
}

void resize(int width, int height) {
    static constexpr float baseRatio = BASE_WINDOW_WIDTH / BASE_WINDOW_HEIGHT;
    const float windowRatio = static_cast<float>(width) / static_cast<float>(height);
    SDL_Rect viewportRect;

    if(width >= baseRatio * height) {
        const int size = static_cast<int>(static_cast<float>(height) * baseRatio);
        const int offset = static_cast<int>((width - size) * 0.5f);
        viewportRect = {offset, 0, size, height};
    } else {
        const int size = static_cast<int>(static_cast<float>(width) / baseRatio);
        const int offset = static_cast<int>((height - size) * 0.5f);
        viewportRect = {0, offset, width, size};
    }

    SDL_RenderSetLogicalSize(renderer, BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT);
}

void setupBalls() {
    constexpr uint8_t numbers[15] = { 1, 14, 2, 15, 3, 13, 8, 6, 12, 7, 10, 4, 9, 11, 5};
    const glm::vec2 position(table.getSize().x * 0.2, 0.0f);

    for(Ball &ball : balls)
        ball.m_Velocity = {0,0};

    static constexpr float width = std::sqrt(3.0f) * Ball::RADIUS + Physics::COLLISION_MARGIN;
    static constexpr float height = Ball::DIAMETER + Physics::COLLISION_MARGIN;

    int idx = 0;
    for(int row=-2; row<=2; ++row) {
        for(int col=row; col<=2; ++col) {
            balls[numbers[idx++]].m_Position = position + glm::vec2(
                col * width,
                row * height - col * Ball::RADIUS + Ball::DIAMETER 
            );
        }
    }

    cueBall.m_Position = glm::vec2(-table.getSize().x * (1.0 - INIT_CUE_BALL_POSITION_NORMALIZED) * 0.5f, 0.0f);
}

void handleEvent(const SDL_Event *event) {
    switch(event->type) {
        case SDL_QUIT:
            quit = true;
            break;

        case SDL_WINDOWEVENT: {
            switch(event->window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    resize(event->window.data1, event->window.data2);
                    break;
            }
            break;
        }

        case SDL_MOUSEBUTTONDOWN: {
            glm::ivec2 pos;
            SDL_GetRelativeMouseState(&pos.x, &pos.y);
            if(cueBall.isPointOverlapping(pos)) {
                isCueBallHeld = true;
                cueBallHeldStartPosition = pos;
            }
            break;
        }

        case SDL_MOUSEBUTTONUP:
            if(isCueBallHeld) {
                isCueBallHeld = false;

                glm::ivec2 pos;
                SDL_GetRelativeMouseState(&pos.x, &pos.y);

                const glm::vec2 delta = cueBallHeldStartPosition - glm::vec2(pos);
                const float force = delta.length();
                const glm::vec2 direction = glm::normalize(delta);

                cueHitCueBall(direction, force);
            }
            break;

        case SDL_KEYDOWN:
            if(event->key.repeat)
                break;

            switch(event->key.keysym.sym) {
                case SDLK_i:
                    // view.zoom(1.1);
                    // window->setView(view);
                    break;

                case SDLK_o:
                    //view.zoom(0.9);
                    //window->setView(view);
                    break;

                case SDLK_r:
                    setupBalls();
                    break;

                default: break;
            }
            break;

        default: break;
    }
}

void init() {
    Audio::init();
    Random::init();

    for(int i=0; i<=15; ++i) balls.emplace_back(i);
    setupBalls();

    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        std::cerr << "Failed to init SDL2: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
        return;
    }

    camera = glm::vec2(-BASE_WINDOW_WIDTH * 0.5f, -BASE_WINDOW_HEIGHT * 0.5f);

    window = SDL_CreateWindow("Billiard by rxn7", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    resize(BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT);
}

void cleanup() {
    Audio::cleanup();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
