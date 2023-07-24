#include "physics.h"
#include "audio.h"
#include "collision.h"
#include "game.h"
#include "mathUtils.h"
#include "pocket.h"
#include "random.h"

#include <chrono>
#include <iostream>
#include <thread>

typedef std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::duration<float>> TimePoint;

static std::vector<Ball> *s_Balls;
static float s_Accumulator = 0.0f;

void Physics::init(std::vector<Ball> *balls) {
	s_Balls = balls;
}

void Physics::update(float dt) {
	const float timeStep = 1.0f / Physics::stepsPerSecond;

	s_Accumulator += dt;
	while (s_Accumulator >= timeStep) {
		step(timeStep);
		s_Accumulator -= timeStep;
	}
}

void Physics::step(float ts) {
	std::vector<Collision> collisions;

	for (Ball &ball : *s_Balls) {
		if (ball.m_InPocket)
			continue;

		if (Pocket::isBallInsideAny(ball)) {
			ball.pocket();
			continue;
		}

		if (MathUtils::lengthSqr(ball.m_Velocity) <= 3) {
			ball.m_Velocity = {0, 0};
		} else {
			const sf::Vector2f movement = ball.m_Velocity * ts;
			ball.m_Position += movement;
			ball.applyDrag(ts);
		}

		for (Ball &target : *s_Balls) {
			if (ball.getNumber() == target.getNumber() || target.m_InPocket)
				continue;

			resolveCollision(ball, target);
		}

		const Table &table = Game::getInstance().getTable();
		resolveTableCollision(ball, table.isBallOverlapping(ball), table);
	}
}

void Physics::resolveCollision(Ball &a, Ball &b) {
	sf::Vector2f positionDelta = a.m_Position - b.m_Position;
	float distanceSquared = MathUtils::lengthSqr(positionDelta);

	if (distanceSquared > Ball::DIAMETER_SQR)
		return;

	float distance = glm::sqrt(distanceSquared);
	const float overlap = (distance - Ball::DIAMETER) * 0.5f / distance;

	const sf::Vector2f correction = positionDelta * overlap;
	a.m_Position -= correction;
	b.m_Position += correction;

	positionDelta = a.m_Position - b.m_Position;
	distanceSquared = MathUtils::lengthSqr(positionDelta);
	distance = glm::sqrt(distanceSquared);

	const sf::Vector2f relativeVelocity = b.m_Velocity - a.m_Velocity;
	const sf::Vector2f normal = positionDelta / distance;

	const float impulse = glm::dot<2, float>(glm::vec2(relativeVelocity.x, relativeVelocity.y), glm::vec2(normal.x, normal.y)) * 2.0f / (Ball::MASS * 2.0f);
	const sf::Vector2f force = impulse * normal;

	a.m_Velocity += force;
	b.m_Velocity -= force;

	// const float forcePercentage = std::clamp(std::fabs(force) / 1000.0f, 0.0f, 1.0f);
	// Audio::play(a.m_Sound, Audio::AudioType::BALL_WITH_BALL_COLLISION, forcePercentage * 100.0f, (1.0f + forcePercentage) * 0.5f);
}

void Physics::resolveTableCollision(Ball &ball, const Table::OverlapResult &result, const Table &table) {
	if (!result.first)
		return;

	float margin = table.getMargin();

	if (result.second.x > 0)
		ball.m_Position.x = -table.getSize().x * 0.5f + Ball::RADIUS + margin;
	else if (result.second.x < 0)
		ball.m_Position.x = table.getSize().x * 0.5f - Ball::RADIUS - margin;

	if (result.second.y > 0)
		ball.m_Position.y = -table.getSize().y * 0.5f + Ball::RADIUS + margin;
	else if (result.second.y < 0)
		ball.m_Position.y = table.getSize().y * 0.5f - Ball::RADIUS - margin;

	if (std::fabs(result.second.x) > 0.1f)
		ball.m_Velocity.x *= -1.0f;
	if (std::fabs(result.second.y) > 0.1f)
		ball.m_Velocity.y *= -1.0f;

	const float velocityPercentage = std::clamp(MathUtils::length(ball.m_Velocity) / 1000.0f, 0.0f, 1.0f);
	Audio::play(ball.m_Sound, Audio::AudioType::BALL_WITH_TABLE_COLLISION, velocityPercentage * 100.0f, (1.0f + velocityPercentage) * 0.5f);
}
