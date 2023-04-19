#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

#include "ball.h"
#include "cue.h"
#include "gameOptions.h"
#include "imGuiLayer.h"
#include "lightingProperties.h"
#include "performanceStats.h"

class Game {
  public:
	Game();
	~Game();

	void start();
	bool allBallsStopped() const;

  private:
	void update();
	void render();
	void rackBalls();
	void resize(const unsigned int width, const unsigned int height);
	void handleEvent(const sf::Event &event);

  public:
	sf::RenderWindow m_Window;

  private:
	friend ImGuiLayer;
	GameOptions m_Options;

	friend ImGuiLayer;
	LightingProperties m_LightProps;

	PerformanceStats m_PerfStats;

	Ball *mp_CueBall;
	Table m_Table;
	std::vector<Ball> m_Balls;
	std::unique_ptr<Cue> mp_Cue;

	ImGuiLayer m_ImGuiLayer;

	sf::View m_View;
	sf::Time m_FrameTime;
};
