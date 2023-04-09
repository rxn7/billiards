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

void Audio::cleanup() {
    for(auto &[type, chunk] : chunks)
        Mix_FreeChunk(chunk);
}

Mix_Chunk *Audio::getChunk(Audio::AudioType type) {
    const auto it = chunks.find(type);
    assert(it != chunks.end());
    return (*it).second;
}

void Audio::play(AudioType type, float volume) {
    int channel = Mix_PlayChannel(-1, getChunk(type), 0);
    if(channel != -1) {
        Mix_Volume(channel, volume);
    }
}
