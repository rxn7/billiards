#pragma once

#include "lightingProperties.h"
#include "mathUtils.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/vec2.hpp>
#include <memory>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Glsl.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

inline const sf::Color BALL_COLORS[] = {
	sf::Color(255, 255, 255), sf::Color(255, 215, 0), sf::Color(0, 0, 255), sf::Color(255, 0, 0),  sf::Color(128, 0, 128),
	sf::Color(255, 165, 0),	  sf::Color(34, 139, 34), sf::Color(128, 0, 0), sf::Color(25, 25, 25),
};

class Ball {
  public:
	Ball();
	Ball(const uint8_t number);

  public:
	static void init();
	void update(const float dt);
	void render(sf::RenderTarget &renderTarget, const LightingProperties &lightProps) const;
	void renderDebug(sf::RenderTarget &renderTarget) const;
	void pocket();
	void applyDrag(const float dt);

	static const sf::Color &getColor(const int number) {
		return number == 0 || number == 8 ? BALL_COLORS[number] : BALL_COLORS[(number) % 8];
	}

	inline bool isStriped() const {
		return m_Number > 8;
	}

	inline bool isPointOverlapping(const sf::Vector2f &v, float radius = RADIUS) const {
		const sf::Vector2f delta = m_Position - v;
		return MathUtils::lengthSqr(delta) < radius * radius;
	}

	inline uint8_t getNumber() const {
		return m_Number;
	}

	inline bool hasStopped() const {
		return m_Stopped || m_InPocket;
	}

  private:
	void applyRotation(const sf::Vector2f &movement, const float dt);

  public:
	static constexpr float RADIUS = 15.0f;
	static constexpr float RADIUS_SQR = RADIUS * RADIUS;
	static constexpr float DIAMETER = RADIUS * 2.0f;
	static constexpr float DIAMETER_SQR = DIAMETER * DIAMETER;
	static constexpr float DRAG_COEFFICIENT = 0.40f;
	static constexpr float MASS = 5.500f;
	static constexpr float CUE_MASS = 6.000f;

	static sf::Shader s_Shader;
	bool m_Stopped = false;
	sf::Vector2f m_Velocity = {0, 0};
	sf::Vector2f m_Position = {0, 0};
	glm::quat m_Rotation;
	float m_Scale = 1.0f;
	sf::Sound m_Sound;
	bool m_InPocket = false;

  private:
	const sf::Color &m_Color;
	uint8_t m_Number = 8;
};
