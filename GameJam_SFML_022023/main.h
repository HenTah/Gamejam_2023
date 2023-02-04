#pragma once

#define _USE_MATH_DEFINES

constexpr auto GAME_NAME = "PLANT KILLER";

constexpr auto PLAYER_TEXTURE = "./assets/player.png";
constexpr auto ROOT_TEXTURE = "./assets/root.png";
constexpr auto BG_TEXTURE = "./assets/bg9.png";
constexpr auto AUDIO_MUSIC = "./assets/music.ogg";
constexpr auto AUDIO_HIT = "./assets/hit.wav";
constexpr auto AUDIO_MISS = "./assets/miss.wav";

constexpr int WIN_W = 1280;
constexpr int WIN_H = 720;

enum e_sounds
{
	SOUND_HIT,
	SOUND_MISS
};

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <chrono>
#include <cmath>
#include <random>
#include <iostream>
#include <random>
#include "plant.h"
#include "player.h"
#include "audio.h"
#include "game.h"
#include "Explosion.h"
