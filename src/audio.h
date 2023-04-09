#pragma once

#include <cinttypes>
#include <SDL2/SDL_mixer.h>

namespace Audio {
    constexpr char BALL_WITH_BALL_COLLISION_SOUND_PATH[] = "res/audio/ball_collision.wav";
    constexpr char BALL_WITH_TABLE_COLLISION_SOUND_PATH[] = "res/audio/table_collision.wav";

    enum AudioType : std::uint8_t {
        BALL_WITH_BALL_COLLISION,
        BALL_WITH_TABLE_COLLISION,
    };

    void init();
    Mix_Chunk *getChunk(AudioType type);
}
