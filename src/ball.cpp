#include "ball.h"
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

static sf::Shader shader;
static sf::VertexArray vertexArray(sf::PrimitiveType::Quads, 4);
static sf::Text text;
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
    sf::Color(0, 0, 0),
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

void Ball::render(sf::RenderWindow &window) const {
    shader.setUniform("u_Number", m_Number);
    shader.setUniform("u_Color", sf::Glsl::Vec4(getColor()));

    sf::Transform transform;
    transform.translate(m_Position);
    transform.scale(RADIUS * m_Scale, RADIUS * m_Scale);

    sf::RenderStates states(sf::BlendAlpha, transform, nullptr, &shader);
    window.draw(vertexArray, states);
}

const sf::Color &Ball::getColor() const {
    return BALL_COLORS[(m_Number-1) % 8];
} 
void Ball::init() {
    initialized = true;
    shader.loadFromFile("res/shaders/ball_frag.glsl", sf::Shader::Type::Fragment);

    sf::Vertex v;
    for(int i=0; i<4; ++i) {
        v.texCoords = UVS[i],
        v.position = UVS[i] * 2.0f - sf::Vector2(1.0f, 1.0f);
        vertexArray.append(v);
    }
}
