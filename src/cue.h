#pragma once

#include "ball.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class Cue {
  public:
	Cue(const sf::RenderWindow &window, Ball *cueBall);
	void render(sf::RenderTarget &renderTarget) const;
	void update(float dt);
	void hitAnimationStep(float dt);
	void startAiming();
	void hit();

  private:
	void actualHit();

  public:
	bool m_Aiming = false;

  private:
	static constexpr float HIT_ANIMATION_SPEED = 2000.0f;
	static constexpr float MIN_FORCE = 1.0f;
	static constexpr float MAX_FORCE = 1000.0f;
	static constexpr float VISUAL_RANGE = 100.0f;

	const sf::RenderWindow &m_Window;
	Ball *mp_CueBall;
	float m_Force;
	bool m_HitAnimation = false;
	sf::Vector2f m_Direction;
	sf::Sprite m_Sprite;
	sf::Sound m_Sound;
	sf::Texture m_Texture;
};
