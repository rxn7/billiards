#include "audio.h"
#include <cassert>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <unordered_map>

static std::unordered_map<Audio::AudioType, sf::SoundBuffer> buffers;

static void addBuffer(Audio::AudioType type, const std::string &path) {
	sf::SoundBuffer buff;
	assert(buff.loadFromFile(path));
	buffers.emplace(type, buff);
}

void Audio::init() {
	addBuffer(AudioType::BALL_WITH_BALL_COLLISION, "assets/audio/ball_collision.wav");
	addBuffer(AudioType::BALL_WITH_TABLE_COLLISION, "assets/audio/table_collision.wav");
	addBuffer(AudioType::CUE_HIT, "assets/audio/cue_hit.wav");
	addBuffer(AudioType::POCKET, "assets/audio/pocket.wav");
	addBuffer(AudioType::TURN, "assets/audio/turn.wav");
}

sf::SoundBuffer &Audio::getSoundBuffer(Audio::AudioType type) {
	const auto &it = buffers.find(type);
	assert(it != buffers.end());
	return it->second;
}

void Audio::play(sf::Sound &sound, AudioType type, float volume, float pitch) {
	const sf::SoundBuffer &buf = getSoundBuffer(type);

	sound.setBuffer(buf);
	sound.setVolume(volume);
	sound.setPitch(pitch);
	sound.play();
}
