#pragma once

#include "lightingProperties.h"
#include "windowProperties.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

struct Ball;
struct LightingProperties;

class Table {
  public:
	Table(const sf::Vector2f &size = {DEFAULT_WIDTH, DEFAULT_HEIGHT}, const sf::Color &color = {0, 98, 162});
	static void init();

	void render(sf::RenderTarget &renderTarget, const std::vector<Ball> &balls, const LightingProperties &lightProps) const;
	std::pair<bool, sf::Vector2f> isBallOverlapping(const Ball &ball) const;

	inline float getMargin() const {
		return TEXTURE_MARGIN * m_Sprite.getScale().x;
	}
	inline const sf::Vector2f &getSize() const {
		return m_Size;
	}

  public:
	static constexpr float DEFAULT_WIDTH = WindowProperties::WINDOW_BASE_WIDTH * 0.75f;
	static constexpr float DEFAULT_HEIGHT = DEFAULT_WIDTH * 0.5f;
	static constexpr float TEXTURE_MARGIN = 9.0f;

  private:
	sf::Vector2f m_Size;
	sf::Color m_Color;
	sf::Sprite m_Sprite;
	sf::Texture m_Texture;
	static sf::Shader s_SimpleShader, s_AdvancedShader;
};
