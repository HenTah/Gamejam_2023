#pragma once
#include "main.h"

constexpr int	DEFAULT_VOLUME = 10.f;

class Audio
{
public:
	Audio();
	void	init();
	void	play_music();
	void	play_sound(int sound);

private:
	int					_volume;
	sf::Music			_music;
	sf::SoundBuffer		_buffer_hit;
	sf::SoundBuffer		_buffer_miss;
	sf::Sound			_sound_hit;
	sf::Sound			_sound_miss;
};
