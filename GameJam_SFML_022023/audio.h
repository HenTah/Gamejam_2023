#pragma once
#include "main.h"

constexpr float	DEFAULT_VOLUME = 10.f;
constexpr float	MAX_VOLUME = 50.f;

class Audio
{
public:
	Audio();
	void	init();
	void	play_music();
	void	play_sound(int sound);
	float	get_volume();
	void	set_volume(float volume);

private:
	float				_volume;
	sf::Music			_music;
	sf::SoundBuffer		_buffer_hit;
	sf::SoundBuffer		_buffer_miss;
	sf::Sound			_sound_hit;
	sf::Sound			_sound_miss;
};
