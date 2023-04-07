#include "audio.h"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <unordered_map>
#include <cassert>

static std::unordered_map<Audio::AudioType, sf::SoundBuffer> buffers;
static sf::Sound sounds[10];

static void addBuffer(Audio::AudioType type, const std::string &path) {
    sf::SoundBuffer buff;
    assert(buff.loadFromFile(path));
    buffers.emplace(type, buff);
}

void Audio::init() {
    addBuffer(AudioType::BALL_WITH_BALL_COLLISION, BALL_WITH_BALL_COLLISION_SOUND_PATH);
    addBuffer(AudioType::BALL_WITH_TABLE_COLLISION, BALL_WITH_TABLE_COLLISION_SOUND_PATH);
}

sf::SoundBuffer &Audio::getSoundBuffer(Audio::AudioType type) {
    const auto it = buffers.find(type);
    assert(it != buffers.end());
    return (*it).second;
}

void Audio::play(Audio::AudioType type, float pitch, float volume) {
    sf::Sound *sound = nullptr;
    for(sf::Sound &snd : sounds) {
        if(snd.getStatus() == sf::Sound::Playing)
            return;
        sound = &snd;
    }

    if(sound == nullptr)
        sound = &sounds[0];

    sound->setPitch(pitch);
    sound->setVolume(volume);
    sound->setBuffer(getSoundBuffer(type));
    sound->setAttenuation(0.0f);
    sound->setPlayingOffset(sf::Time::Zero);
    sound->play();
}
