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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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

Ball::Ball(const uint8_t number) : m_Number(number), m_Color(getColor(number)) {
    assert(number >= 0 && number <= 15);

    if(!initialized)
        init();

    m_Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}

void Ball::update(const float dt) {
    sf::Vector2f movement = m_Velocity * dt;
    const float speed = MathUtils::length(m_Velocity);

    m_Position += movement;
    applyDrag(speed, dt);
    applyRotation(speed, movement, dt);
}

void Ball::render(sf::RenderTarget &renderTarget) const {
    shader.setUniform("u_Color", sf::Glsl::Vec4(m_Color));
    shader.setUniform("u_Number", m_Number);
    shader.setUniform("u_RotationMatrix", sf::Glsl::Mat3(glm::value_ptr(glm::mat3_cast(m_Rotation))));
    shader.setUniform("u_NumbersTexture", numbersTexture);

    sf::Transform transform;
    transform.translate(m_Position);
    transform.scale(RADIUS * m_Scale, RADIUS * m_Scale);

    const sf::RenderStates states(sf::BlendAlpha, transform, nullptr, &shader);
    renderTarget.draw(vertexArray, states);
}

void Ball::applyDrag(const float speed, const float dt) {
    const sf::Vector2f dragDirection = -MathUtils::normalized(m_Velocity);
    const sf::Vector2f dragForce = dragDirection * DRAG_COEFFICIENT * speed;

    m_Velocity += dragForce * dt;
}

void Ball::applyRotation(const float speed, const sf::Vector2f &movement, float dt) {
    if(speed == 0.0f)
        return;

    const glm::quat rotationX = glm::angleAxis(-movement.y / RADIUS, glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::quat rotationY = glm::angleAxis(movement.x / RADIUS, glm::vec3(0.0f, 1.0f, 0.0f));

    m_Rotation = rotationX * rotationY * m_Rotation;
}

const sf::Color &Ball::getColor(int number) {
    if(number == 0 || number == 8)
        return BALL_COLORS[number];

    return BALL_COLORS[(number) % 8];
} 

void Ball::init() {
    initialized = true;

    const char *fragShader = {
        #include "../shaders_out/ball.frag.glsl"
    };

    assert(shader.loadFromMemory(fragShader, sf::Shader::Type::Fragment));
    assert(numbersTexture.loadFromFile("assets/textures/numbers.gif"));

    const sf::Vector2f uvs[] = { {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };

    sf::Vertex v;
    for(int i=0; i<4; ++i) {
        v.texCoords = uvs[i],
        v.position = uvs[i] * 2.0f - sf::Vector2f(1.0f, 1.0f);
        vertexArray.append(v);
    }
}
