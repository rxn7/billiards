#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>
#include <vector>

#include "ball.h"
#include "cue.h"
#include "gameOptions.h"
#include "imGuiLayer.h"
#include "lightingProperties.h"
#include "performanceStats.h"
#include "table.h"

class Game {
  public:
	Game();
	~Game();

	static inline Game &getInstance() {
		return *s_Instance;
	}

	inline const Table &getTable() const {
		return m_Table;
	}

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
	static Game *s_Instance;

	bool m_AllBallsStopedLastFrame = true;
	bool m_AllBallsStoped = true;

	friend ImGuiLayer;
	GameOptions m_Options;

	friend ImGuiLayer;
	LightingProperties m_LightProps;

	friend Ball;
	std::vector<Ball> m_Balls;
	Ball *mp_CueBall;

	PerformanceStats m_PerfStats;

	Table m_Table;
	std::unique_ptr<Cue> mp_Cue;

	ImGuiLayer m_ImGuiLayer;

	sf::View m_View;
	sf::Time m_FrameTime;
	sf::Sound m_Sound;
};
