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

			const float distanceSquared = MathUtils::lengthSqr(ball.m_Position - target.m_Position);
			if (distanceSquared + Physics::COLLISION_MARGIN >= Ball::DIAMETER * Ball::DIAMETER)
				continue;

			const float distance = std::sqrt(distanceSquared);
			const float overlap = (distance - Ball::DIAMETER) * 0.5f / distance;

			const sf::Vector2f displace = (ball.m_Position - target.m_Position) * overlap;
			ball.m_Position -= displace;
			target.m_Position += displace;

			collisions.emplace_back(&ball, &target);
		}

		const Table &table = Game::getInstance().getTable();
		resolveTableCollision(ball, table.isBallOverlapping(ball), table);
	}

	for (const Collision &col : collisions)
		resolveCollision(*col.ball, *col.target);
}

void Physics::resolveCollision(Ball &a, Ball &b) {
	const sf::Vector2f positionDelta = a.m_Position - b.m_Position;
	const sf::Vector2f velocityDelta = a.m_Velocity - b.m_Velocity;
	const float distance = MathUtils::length(positionDelta);
	const sf::Vector2f normal = positionDelta / distance;

	const float force = 2.0f * (normal.x * velocityDelta.x + normal.y * velocityDelta.y) / (Ball::MASS * 2.0f);
	const sf::Vector2f forceVector = force * Ball::MASS * normal;

	a.m_Velocity -= forceVector;
	b.m_Velocity += forceVector;

	const float forcePercentage = std::clamp(std::fabs(force) / 1000.0f, 0.0f, 1.0f);
	Audio::play(a.m_Sound, Audio::AudioType::BALL_WITH_BALL_COLLISION, forcePercentage * 100.0f, (1.0f + forcePercentage) * 0.5f);
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
