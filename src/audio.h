#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace Audio {
    constexpr char BALL_WITH_BALL_COLLISION_SOUND_PATH[] = "res/audio/ball_collision.wav";
    constexpr char BALL_WITH_TABLE_COLLISION_SOUND_PATH[] = "res/audio/table_collision.wav";

    enum AudioType : uint8_t {
        BALL_WITH_BALL_COLLISION,
        BALL_WITH_TABLE_COLLISION,
    };

    void init();
    sf::SoundBuffer &getSoundBuffer(AudioType type);
}
