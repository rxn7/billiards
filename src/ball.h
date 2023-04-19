#pragma once

#include "lightingProperties.h"
#include "mathUtils.h"
#include "table.h"
#include <glm/gtc/quaternion.hpp>
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

	inline float getSpeed() const {
		return m_Speed;
	}

  private:
	void applyDrag(const float speed, const float dt);
	void applyRotation(const float speed, const sf::Vector2f &movement, const float dt);

  public:
	static constexpr float RADIUS = 15.0f;
	static constexpr float DIAMETER = RADIUS * 2.0f;
	static constexpr float DRAG_COEFFICIENT = 0.40f;
	static constexpr float MASS = 0.160f;

	static sf::Shader s_Shader;
	sf::Vector2f m_Velocity = {0, 0};
	sf::Vector2f m_Position = {0, 0};
	float m_Scale = 1.0f;
	sf::Sound m_Sound;
	bool m_InPocket = false;

  private:
	float m_Speed;
	glm::quat m_Rotation;
	const sf::Color &m_Color;
	uint8_t m_Number = 8;
};
