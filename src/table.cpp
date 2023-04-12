#include "table.h"
#include "ball.h"
#include "lightingProperties.h"
#include "mathUtils.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>

sf::Shader Table::s_AdvancedShader, Table::s_SimpleShader;

Table::Table(const sf::Vector2f &size, const sf::Color &color) : m_Size(size), m_Color(color) {
    assert(m_Texture.loadFromFile("assets/textures/table.png"));

    const sf::Vector2f textureSize(m_Texture.getSize());
    const float scale = size.x / textureSize.x;

    m_Sprite.setTexture(m_Texture, true);
    m_Sprite.setScale(scale, scale);
    m_Sprite.setPosition(-size * 0.5f);
}

void Table::init() {
    assert(s_AdvancedShader.loadFromFile("assets/shaders/table.frag.glsl", sf::Shader::Fragment));
    s_AdvancedShader.setUniform("u_BallRadiusSqr", Ball::RADIUS * Ball::RADIUS);

    assert(s_SimpleShader.loadFromFile("assets/shaders/table.simple.frag.glsl", sf::Shader::Fragment));
}

void Table::render(sf::RenderTarget &renderTarget, const std::vector<Ball> &balls, const LightingProperties &lightProps) const {
    sf::Shader *shader = &s_SimpleShader;
    if(lightProps.shadowsEnabled) {
        shader = &s_AdvancedShader;
        sf::Glsl::Vec2 ballsPositions[16];
        std::uint8_t idx = 0;
        for(const Ball &ball : balls)
            if(!ball.m_InPocket)
                ballsPositions[idx++] = ball.m_Position;

        shader->setUniform("u_BallCount", idx);
        shader->setUniformArray("u_BallPositions", ballsPositions, 16);
    }

    shader->setUniform("u_LightPosition", lightProps.lightPosition);
    shader->setUniform("u_LightColor", lightProps.lightColor);
    shader->setUniform("u_DiffuseIntensity", lightProps.diffuseIntensity);
    shader->setUniform("u_AmbientIntensity", lightProps.ambientIntensity);
    shader->setUniform("u_Size", m_Size);

    renderTarget.draw(m_Sprite, shader);
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
