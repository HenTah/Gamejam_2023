#include "main.h"

Audio::Audio()
{
}

void	Audio::init()
{
	if (!_music.openFromFile(AUDIO_MUSIC))
		exit(NULL);
	if (!_buffer_hit.loadFromFile(AUDIO_HIT))
		exit(NULL);
	if (!_buffer_miss.loadFromFile(AUDIO_MISS))
		exit(NULL);
	_volume = DEFAULT_VOLUME;
	_music.setVolume(_volume);
	_sound_hit.setVolume(_volume);
	_sound_miss.setVolume(_volume);
}

void	Audio::play_music()
{
	_music.play();
}

void	Audio::play_sound(int sound)
{
	switch (sound)
	{
	case SOUND_HIT:
		if (_sound_hit.getStatus() == sf::Sound::Playing)
			return;
		_sound_hit.setBuffer(_buffer_hit);
		_sound_hit.play();
		break;
	case SOUND_MISS:
		if (_sound_miss.getStatus() == sf::Sound::Playing)
			return;
		_sound_miss.setBuffer(_buffer_miss);
		_sound_miss.play();
		break;
	}
}

int	Audio::get_volume()
{
	return (_volume);
}

void	Audio::set_volume(int volume)
{
	_volume = volume;
	_music.setVolume(_volume);
	_sound_hit.setVolume(_volume);
	_sound_miss.setVolume(_volume);
}