#include "table.h"
#include "ball.h"
#include "lightingProperties.h"
#include "mathUtils.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>

sf::Shader Table::s_Shader;

Table::Table(const sf::Vector2f &size, const sf::Color &color) : m_Size(size), m_Color(color) {
    assert(s_Shader.loadFromFile("assets/shaders/table.frag.glsl", sf::Shader::Fragment));
    s_Shader.setUniform("u_BallRadiusSqr", Ball::RADIUS * Ball::RADIUS);
    assert(m_Texture.loadFromFile("assets/textures/table.png"));

    const sf::Vector2f textureSize(m_Texture.getSize());
    float scale = size.x / textureSize.x;

    m_Sprite.setTexture(m_Texture, true);
    m_Sprite.setScale(scale, scale);
    m_Sprite.setPosition(-size * 0.5f);
}

void Table::render(sf::RenderTarget &renderTarget, const std::vector<Ball> &balls, const LightingProperties &lightProps) const {
    sf::Glsl::Vec2 ballsPositions[16];
    std::uint8_t idx = 0;
    for(const Ball &ball : balls)
        if(!ball.m_InPocket)
            ballsPositions[idx++] = ball.m_Position;

    s_Shader.setUniform("u_LightPosition", lightProps.lightPosition);
    s_Shader.setUniform("u_LightColor", lightProps.lightColor);
    s_Shader.setUniform("u_DiffuseIntensity", lightProps.diffuseIntensity);
    s_Shader.setUniform("u_AmbientIntensity", lightProps.ambientIntensity);
    s_Shader.setUniform("u_BallCount", idx);
    s_Shader.setUniform("u_Size", m_Size);
    s_Shader.setUniformArray("u_BallPositions", ballsPositions, 16);

    renderTarget.draw(m_Sprite, &s_Shader);
}

std::pair<bool, sf::Vector2f> Table::isBallOverlapping(const Ball &ball) const {
    bool overlapping = false;
    sf::Vector2f normal = {0.0f, 0.0f};

    const float margin = getMargin();

    if(ball.m_Position.x - Ball::RADIUS < -m_Size.x * 0.5f + margin) {
        overlapping = true;
        normal += sf::Vector2f(1.0f, 0.0f);
    } else if(ball.m_Position.x + Ball::RADIUS > m_Size.x * 0.5f - margin) {
        overlapping = true;
        normal += sf::Vector2f(-1.0f, 0.0f);
    }
    if(ball.m_Position.y - Ball::RADIUS < -m_Size.y * 0.5f + margin) {
        overlapping = true;
        normal += sf::Vector2f(0.0f, 1.0f);
    } else if(ball.m_Position.y + Ball::RADIUS > m_Size.y * 0.5f - margin) {
        overlapping = true;
        normal += sf::Vector2f(0.0f, -1.0f);
    }

    normal = MathUtils::normalized(normal);

    return std::make_pair(overlapping, normal);
}
