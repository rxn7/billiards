#include "audio.h"
#include "SDL_mixer.h"
#include <string>
#include <unordered_map>
#include <cassert>

static std::unordered_map<Audio::AudioType, Mix_Chunk*> chunks;

static void addChunk(Audio::AudioType type, const std::string &path) {
    chunks.emplace(type, Mix_LoadWAV(path.c_str()));
}

void Audio::init() {
    addChunk(AudioType::BALL_WITH_BALL_COLLISION, BALL_WITH_BALL_COLLISION_SOUND_PATH);
    addChunk(AudioType::BALL_WITH_TABLE_COLLISION, BALL_WITH_TABLE_COLLISION_SOUND_PATH);
}

Mix_Chunk *Audio::getChunk(Audio::AudioType type) {
    const auto it = chunks.find(type);
    assert(it != chunks.end());
    return (*it).second;
}