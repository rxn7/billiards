#pragma once

#include <SFML/System/Vector2.hpp>

struct Ball;

struct Collision {
    Ball *ball;
    Ball *target;
};
