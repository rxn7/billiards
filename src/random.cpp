#include "random.h"
#include <random>

static std::random_device device;
static std::mt19937 mt;

void Random::init() {
    mt = std::mt19937(device());
}

float Random::rangeF(float min, float max) {
    return std::uniform_real_distribution<float>(min, max)(mt);
}

int Random::rangeI(int min, int max) {
    return std::uniform_int_distribution<int>(min, max)(mt);
}
