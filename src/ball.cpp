#include "ball.h"
#include <cassert>

Ball::Ball(uint8_t number) : m_Number(number) {
    assert(number >= 1 && number <= 12);
}
