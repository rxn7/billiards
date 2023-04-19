#include "ball.h"
#include "audio.h"
#include "collision.h"
#include "lightingProperties.h"
#include "mathUtils.h"
#include <cassert>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Glsl.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>

sf::Shader Ball::s_Shader;
static sf::VertexArray vertexArray(sf::PrimitiveType::Quads, 4);
static sf::Texture numbersTexture;
static sf::RectangleShape debugShape;

Ball::Ball(const uint8_t number) : m_Number(number), m_Color(getColor(number)) {
	assert(number >= 0 && number <= 15);
	m_Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}

void Ball::update(const float dt) {
	if (m_InPocket)
		return;

	const float speed = MathUtils::length(m_Velocity);
	if (speed <= 3) {
		m_Velocity = {0, 0};
		return;
	}

	const sf::Vector2f movement = m_Velocity * dt;

	m_Position += movement;
	applyDrag(speed, dt);
	applyRotation(speed, movement, dt);
}

void Ball::render(sf::RenderTarget &renderTarget, const LightingProperties &lightProps) const {
	if (m_InPocket)
		return;

	const sf::Glsl::Vec3 deltaToLight = lightProps.lightPosition - sf::Vector3f(m_Position.x, m_Position.y, 0.0f);

	s_Shader.setUniform("u_LightToBallDirection", MathUtils::normalized(deltaToLight));
	s_Shader.setUniform("u_Color", MathUtils::colorToGlslVec3(m_Color));
	s_Shader.setUniform("u_Number", m_Number);
	s_Shader.setUniform("u_RotationMatrix", sf::Glsl::Mat3(glm::value_ptr(glm::mat3_cast(m_Rotation))));

	sf::Transform transform;
	transform.translate(m_Position);
	transform.scale(RADIUS * m_Scale, RADIUS * m_Scale);

	const sf::RenderStates states(sf::BlendAlpha, transform, nullptr, &s_Shader);
	renderTarget.draw(vertexArray, states);
}

void Ball::renderDebug(sf::RenderTarget &renderTarget) const {
	if (m_InPocket)
		return;

	debugShape.setSize(sf::Vector2f(MathUtils::length(m_Velocity), 2.0f));
	debugShape.setPosition(m_Position);
	debugShape.setRotation(glm::degrees(std::atan2(m_Velocity.y, m_Velocity.x)));
	renderTarget.draw(debugShape);
}

void Ball::applyDrag(const float speed, const float dt) {
	const sf::Vector2f dragDirection = -MathUtils::normalized(m_Velocity);
	sf::Vector2f dragForce = dragDirection * DRAG_COEFFICIENT * speed;

	m_Velocity += dragForce * dt;
}

void Ball::applyRotation(const float speed, const sf::Vector2f &movement, float dt) {
	if (speed == 0.0f)
		return;

	const glm::quat rotationX = glm::angleAxis(-movement.y / RADIUS, glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::quat rotationY = glm::angleAxis(movement.x / RADIUS, glm::vec3(0.0f, 1.0f, 0.0f));

	m_Rotation = rotationX * rotationY * m_Rotation;
}

void Ball::pocket() {
	Audio::play(m_Sound, Audio::AudioType::POCKET);

	if (m_Number == 0) {
		m_Position = sf::Vector2f(0, 0);
		m_Velocity = sf::Vector2f(0, 0);
		return;
	}

	m_InPocket = true;
}

void Ball::init() {
	assert(s_Shader.loadFromFile("assets/shaders/ball.frag.glsl", sf::Shader::Type::Fragment));
	assert(numbersTexture.loadFromFile("assets/textures/numbers.gif"));
	s_Shader.setUniform("u_NumbersTexture", numbersTexture);

	const sf::Vector2f uvs[] = {{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}};

	sf::Vertex v;
	for (int i = 0; i < 4; ++i) {
		v.texCoords = uvs[i], v.position = uvs[i] * 2.0f - sf::Vector2f(1.0f, 1.0f);
		vertexArray.append(v);
	}

	debugShape.setFillColor(sf::Color::Blue);
}
