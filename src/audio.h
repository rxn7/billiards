#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace Audio {
    constexpr char BALL_WITH_BALL_COLLISION_SOUND_PATH[] = "res/audio/ball_collision.wav";
    constexpr char BALL_WITH_TABLE_COLLISION_SOUND_PATH[] = "res/audio/table_collision.wav";
    constexpr char CUE_HIT_SOUND_PATH[] = "res/audio/cue_hit.wav";

    enum AudioType : uint8_t {
        BALL_WITH_BALL_COLLISION,
        BALL_WITH_TABLE_COLLISION,
        CUE_HIT,
    };

    void init();
    sf::SoundBuffer &getSoundBuffer(AudioType type);
    void play(sf::Sound &sound, AudioType type, float volume=100.0f, float pitch=1.0f);
}
