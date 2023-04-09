#include "audio.h"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <unordered_map>
#include <cassert>

static std::unordered_map<Audio::AudioType, sf::SoundBuffer> buffers;

static void addBuffer(Audio::AudioType type, const std::string &path) {
    sf::SoundBuffer buff;
    assert(buff.loadFromFile(path));
    buffers.emplace(type, buff);
}

void Audio::init() {
    addBuffer(AudioType::BALL_WITH_BALL_COLLISION, BALL_WITH_BALL_COLLISION_SOUND_PATH);
    addBuffer(AudioType::BALL_WITH_TABLE_COLLISION, BALL_WITH_TABLE_COLLISION_SOUND_PATH);
    addBuffer(AudioType::CUE_HIT, CUE_HIT_SOUND_PATH);
}

sf::SoundBuffer &Audio::getSoundBuffer(Audio::AudioType type) {
    const auto it = buffers.find(type);
    assert(it != buffers.end());
    return (*it).second;
}

void Audio::play(sf::Sound &sound, AudioType type, float volume, float pitch) {
    sound.setBuffer(getSoundBuffer(type));
    sound.setVolume(volume);
    sound.setPitch(pitch);
    sound.play();
}
