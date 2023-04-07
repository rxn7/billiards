#include "ball.h"
#include "audio.h"
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


Ball::Ball(uint8_t number) : m_Number(number), m_Color(getColor(number)) {
    assert(number >= 0 && number <= 15);

    if(!initialized)
        init();

    calculateRotationMatrix();
}

void Ball::update(float dt) {
    sf::Vector2f movement = m_Velocity * dt;
    float speed = MathUtils::length(m_Velocity);

    m_Position += movement;
    applyDrag(speed, dt);
    applyRotation(speed, movement, dt);

    calculateRotationMatrix();
}

void Ball::render(sf::RenderTarget &renderTarget) const {
    shader.setUniform("u_Color", sf::Glsl::Vec4(m_Color));
    shader.setUniform("u_Number", m_Number);
    shader.setUniform("u_RotationMatrix", sf::Glsl::Mat3(m_RotationMatrixTransform));
    shader.setUniform("u_NumbersTexture", numbersTexture);

    sf::Transform transform;
    transform.translate(m_Position);
    transform.scale(RADIUS * m_Scale, RADIUS * m_Scale);

    sf::RenderStates states(sf::BlendAlpha, transform, nullptr, &shader);
    renderTarget.draw(vertexArray, states);
}

void Ball::applyDrag(float speed, float dt) {
    sf::Vector2f dragDirection = -MathUtils::normalized(m_Velocity);
    sf::Vector2f dragForce = dragDirection * DRAG_COEFFICIENT * speed;
    m_Velocity += dragForce * dt;
}

void Ball::applyRotation(float speed, const sf::Vector2f &movement, float dt) {
    if(speed == 0.0f)
        return;

    float distanceMoved = MathUtils::length(movement);
    static const sf::Vector3f SURFACE_NORMAL(0,0,1);
    sf::Vector3f rotationAxis = MathUtils::normalized(MathUtils::cross(SURFACE_NORMAL, sf::Vector3f(movement.x, movement.y, 0.0f)));

    m_Rotation += rotationAxis * distanceMoved / RADIUS;
}

void Ball::applyPhysics(std::vector<Ball> &balls, const Table &table) {
    auto overlapCheck = [](const Ball &a, const Ball &b) {
        sf::Vector2f delta = a.m_Position - b.m_Position;
        float distSqr = MathUtils::lengthSqr(delta);
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

        std::pair<bool, sf::Vector2f> tableOverlapResult = table.isBallOverlapping(ball);
        if(tableOverlapResult.first) {
            if(tableOverlapResult.second.x > 0)
                ball.m_Position.x = -table.getSize().x * 0.5f + RADIUS;
            else if(tableOverlapResult.second.x < 0)
                ball.m_Position.x = table.getSize().x * 0.5f - RADIUS;

            if(tableOverlapResult.second.y > 0)
                ball.m_Position.y = -table.getSize().y * 0.5f + RADIUS;
            else if(tableOverlapResult.second.y < 0)
                ball.m_Position.y = table.getSize().y * 0.5f - RADIUS;

            if(std::fabs(tableOverlapResult.second.x) > 0.1f) 
                ball.m_Velocity.x *= -1.0f;
            if(std::fabs(tableOverlapResult.second.y) > 0.1f) 
                ball.m_Velocity.y *= -1.0f;

            Audio::play(Audio::AudioType::BALL_WITH_TABLE_COLLISION);
        }
    }

    for(const Collision &col : collisions) {
        sf::Vector2f positionDelta = col.ball->m_Position - col.target->m_Position;
        sf::Vector2f velocityDelta = col.ball->m_Velocity - col.target->m_Velocity;
        float distance = MathUtils::length(positionDelta);
        sf::Vector2f normal = positionDelta / distance;

        float force = 2.0f * (normal.x * velocityDelta.x + normal.y * velocityDelta.y) / (Ball::MASS * 2.0f);
        sf::Vector2f forceVector = force * Ball::MASS * normal;

        col.ball->m_Velocity -= forceVector;
        col.target->m_Velocity += forceVector;
        
        Audio::play(Audio::AudioType::BALL_WITH_BALL_COLLISION);
    }
}

void Ball::calculateRotationMatrix() {
    float cosX = std::cos(m_Rotation.x);
    float sinX = std::sin(m_Rotation.x);
    sf::Transform xRotationMatrix(
        1.0f, 0.0f, 0.0f,
        0.0f, cosX, -sinX,
        0.0f, sinX, cosX
    );
 
    float cosY = std::cos(m_Rotation.y);
    float sinY = std::sin(m_Rotation.y);
    sf::Transform yRotationMatrix(
        cosY, 0.0f, sinY,
        0.0f, 1.0f, 0.0f,
        -sinY, 0.0f, cosY
    );

    float cosZ = std::cos(m_Rotation.z);
    float sinZ = std::sin(m_Rotation.z);
    sf::Transform zRotationMatrix(
        cosZ, -sinZ, 0.0f,
        sinZ, cosZ, 0.0f,
        0.0f, 0.0f, 1.0f
    );

    m_RotationMatrixTransform = xRotationMatrix * yRotationMatrix * zRotationMatrix;
}

const sf::Color &Ball::getColor(int number) {
    if(number == 0 || number == 8)
        return BALL_COLORS[number];

    return BALL_COLORS[(number) % 8];
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
