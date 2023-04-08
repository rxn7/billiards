#include "physics.h"
#include "collision.h"
#include "mathUtils.h"
#include "audio.h"

void Physics::update(std::vector<Ball> &balls, const Table &table) {
    std::vector<Collision> collisions;

    for(Ball &ball : balls)  {
        for(Ball &target : balls) {
            // NOTE: This will not work if there would be multiple balls with the same number 
            if(ball.getNumber() == target.getNumber())
                continue;

            const float distanceSquared = MathUtils::lengthSqr(ball.m_Position - target.m_Position);
            if(distanceSquared + COLLISION_MARGIN >= Ball::DIAMETER * Ball::DIAMETER)
                continue;

            const float distance = std::sqrt(distanceSquared);
            const float overlap = (distance - Ball::DIAMETER) * 0.5f / distance;

            const sf::Vector2f displace = (ball.m_Position - target.m_Position) * overlap;
            ball.m_Position -= displace;
            target.m_Position += displace;

            collisions.emplace_back(
                &ball,
                &target
            );
        }

        const std::pair<bool, sf::Vector2f> tableOverlapResult = table.isBallOverlapping(ball);
        if(tableOverlapResult.first) {
            if(tableOverlapResult.second.x > 0)
                ball.m_Position.x = -table.getSize().x * 0.5f + Ball::RADIUS;
            else if(tableOverlapResult.second.x < 0)
                ball.m_Position.x = table.getSize().x * 0.5f - Ball::RADIUS;

            if(tableOverlapResult.second.y > 0)
                ball.m_Position.y = -table.getSize().y * 0.5f + Ball::RADIUS;
            else if(tableOverlapResult.second.y < 0)
                ball.m_Position.y = table.getSize().y * 0.5f - Ball::RADIUS;

            if(std::fabs(tableOverlapResult.second.x) > 0.1f) 
                ball.m_Velocity.x *= -1.0f;
            if(std::fabs(tableOverlapResult.second.y) > 0.1f) 
                ball.m_Velocity.y *= -1.0f;
        }
    }

    for(const Collision &col : collisions) {
        const sf::Vector2f positionDelta = col.ball->m_Position - col.target->m_Position;
        const sf::Vector2f velocityDelta = col.ball->m_Velocity - col.target->m_Velocity;
        const float distance = MathUtils::length(positionDelta);
        const sf::Vector2f normal = positionDelta / distance;

        const float force = 2.0f * (normal.x * velocityDelta.x + normal.y * velocityDelta.y) / (Ball::MASS * 2.0f);
        const sf::Vector2f forceVector = force * Ball::MASS * normal;

        col.ball->m_Velocity -= forceVector;
        col.target->m_Velocity += forceVector;

        const float volume = std::clamp(std::fabs(force) / 50.0f, 10.0f, 100.0f);
        
        col.ball->m_Sound.setVolume(volume);
        col.ball->m_Sound.setBuffer(Audio::getSoundBuffer(Audio::AudioType::BALL_WITH_BALL_COLLISION));
        col.ball->m_Sound.play();
    }
}