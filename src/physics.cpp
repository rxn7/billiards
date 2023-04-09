#include "physics.h"
#include "collision.h"
#include "audio.h"
#include "random.h"

void Physics::update(std::vector<Ball> &balls, const Table &table) {
    std::vector<Collision> collisions;

    for(Ball &ball : balls)  {
        for(Ball &target : balls) {
            // NOTE: This will not work if there would be multiple balls with the same number 
            if(ball.getNumber() == target.getNumber())
                continue;

            const float distanceSquared = (ball.m_Position - target.m_Position).length();
            if(distanceSquared + COLLISION_MARGIN >= Ball::DIAMETER * Ball::DIAMETER)
                continue;

            const float distance = std::sqrt(distanceSquared);
            const float overlap = (distance - Ball::DIAMETER) * 0.5f / distance;

            const glm::vec2 displace = (ball.m_Position - target.m_Position) * overlap;
            ball.m_Position -= displace;
            target.m_Position += displace;

            collisions.emplace_back(
                &ball,
                &target
            );
        }

        const std::pair<bool, glm::vec2> tableOverlapResult = table.isBallOverlapping(ball);
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

            const float volume = glm::clamp(ball.m_Velocity.length() / 10.0f, 1.0f, 100.0f);
            Audio::play(Audio::AudioType::BALL_WITH_TABLE_COLLISION, volume);
        }
    }

    for(const Collision &col : collisions) {
        const glm::vec2 positionDelta = col.ball->m_Position - col.target->m_Position;
        const glm::vec2 velocityDelta = col.ball->m_Velocity - col.target->m_Velocity;
        const float distance = positionDelta.length();
        const glm::vec2 normal = positionDelta / distance;

        const float force = 2.0f * (normal.x * velocityDelta.x + normal.y * velocityDelta.y) / (Ball::MASS * 2.0f);
        const glm::vec2 forceVector = force * Ball::MASS * normal;

        col.ball->m_Velocity -= forceVector;
        col.target->m_Velocity += forceVector;

        const float volume = glm::clamp(std::fabs(force) / 100.0f, 1.0f, 100.0f);
        Audio::play(Audio::AudioType::BALL_WITH_BALL_COLLISION, volume);
    }
}
