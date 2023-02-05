#pragma once
#include "main.h"

constexpr int	DEFAULT_VOLUME = 10.f;
constexpr int	MAX_VOLUME = 50.f;

class Audio
{
public:
	Audio();
	void	init();
	void	play_music();
	void	play_sound(int sound);
	int		get_volume();
	void	set_volume(int volume);

private:
	int					_volume;
	sf::Music			_music;
	sf::SoundBuffer		_buffer_hit;
	sf::SoundBuffer		_buffer_miss;
	sf::Sound			_sound_hit;
	sf::Sound			_sound_miss;
};
