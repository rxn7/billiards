#pragma once

#include "ball.h"
#include "table.h"

namespace Physics {
    constexpr float COLLISION_MARGIN = 0.0001f;
     
    void update(std::vector<Ball> &ball, const Table &table);
};
