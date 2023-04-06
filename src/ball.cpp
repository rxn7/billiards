#include "ball.h"

#include "mathUtils.h"
#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Glsl.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cassert>
#include <cmath>
#include "collision.h"

static sf::Shader shader;
static sf::VertexArray vertexArray(sf::PrimitiveType::Quads, 4);
static sf::Texture numbersTexture;
static bool initialized = false;

static const sf::Color BALL_COLORS[] = {
    sf::Color(255, 255, 255),
    sf::Color(255, 215, 0),
    sf::Color(0, 0, 255),
    sf::Color(255, 0, 0),
    sf::Color(128, 0, 128),
    sf::Color(255, 165, 0),
    sf::Color(34, 139, 34),
    sf::Color(128, 0, 0),
    sf::Color(25, 25, 25),
};

static const sf::Vector2f UVS[] = {
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
    {1.0f, 0.0f},
};


Ball::Ball(uint8_t number) : m_Number(number) {
    assert(number >= 0 && number <= 15);

    if(!initialized)
        init();
}

void Ball::update(float dt) {
    m_Position += m_Velocity * dt;

    float speed = MathUtils::vecLength(m_Velocity);
    sf::Vector2f dragDirection = -MathUtils::normalized(m_Velocity);
    sf::Vector2f dragForce = dragDirection * DRAG_COEFFICIENT * speed;
    m_Velocity += dragForce * dt;
}

void Ball::render(sf::RenderTarget &renderTarget) const {
    shader.setUniform("u_Color", sf::Glsl::Vec4(getColor()));
    shader.setUniform("u_Number", m_Number);
    shader.setUniform("u_Rotation", m_Rotation);
    shader.setUniform("u_NumbersTexture", numbersTexture);

    sf::Transform transform;
    transform.translate(m_Position);
    transform.scale(RADIUS * m_Scale, RADIUS * m_Scale);

    sf::RenderStates states(sf::BlendAlpha, transform, nullptr, &shader);
    renderTarget.draw(vertexArray, states);
}

void Ball::applyPhysics(std::vector<Ball> &balls) {
    auto overlapCheck = [](const Ball &a, const Ball &b) {
        sf::Vector2f delta = a.m_Position - b.m_Position;
        float distSqr = MathUtils::vecLengthSqr(delta);
        return std::make_tuple(distSqr <= Ball::DIAMETER * Ball::DIAMETER, distSqr);
    };

    std::vector<Collision> collisions;

    for(Ball &ball : balls)  {
        for(Ball &target : balls) {
            // NOTE: This will not work if there would be multiple balls with the same number 
            if(ball.m_Number == target.m_Number)
                continue;

            bool isOverlapping;
            float distanceSquared;
            std::tie(isOverlapping, distanceSquared) = overlapCheck(ball, target);

            if(!isOverlapping)
                continue;

            float distance = std::sqrt(distanceSquared);
            float overlap = (distance - Ball::DIAMETER) * 0.5f / distance;

            sf::Vector2f displace = (ball.m_Position - target.m_Position) * overlap;
            ball.m_Position -= displace;
            target.m_Position += displace;

            collisions.emplace_back(
                &ball,
                &target
            );
        }
    }

    for(const Collision &col : collisions) {
        if(col.target == nullptr) {
            // TODO: Wall collisions
        } else {
            sf::Vector2f positionDelta = col.ball->m_Position - col.target->m_Position;
            sf::Vector2f velocityDelta = col.ball->m_Velocity - col.target->m_Velocity;
            float distance = MathUtils::vecLength(positionDelta);
            sf::Vector2f normal = positionDelta / distance;

            float force = 2.0f * (normal.x * velocityDelta.x + normal.y * velocityDelta.y) / Ball::DIAMETER;
            sf::Vector2f forceVector = force * Ball::RADIUS * normal;

            col.ball->m_Velocity -= forceVector;
            col.target->m_Velocity += forceVector;
        }
    }
}

const sf::Color &Ball::getColor() const {
    if(m_Number == 0)
        return BALL_COLORS[0];

    if(m_Number == 8)
        return BALL_COLORS[8];

    return BALL_COLORS[(m_Number) % 8];
} 

void Ball::init() {
    initialized = true;
    assert(shader.loadFromFile("res/shaders/ball_frag.glsl", sf::Shader::Type::Fragment));
    assert(numbersTexture.loadFromFile("res/numbers.gif"));

    sf::Vertex v;
    for(int i=0; i<4; ++i) {
        v.texCoords = UVS[i],
        v.position = UVS[i] * 2.0f - sf::Vector2f(1.0f, 1.0f);
        vertexArray.append(v);
    }
}
