#pragma once

#define _USE_MATH_DEFINES

constexpr auto GAME_NAME = "PLANT KILLER";

constexpr auto PLAYER_TEXTURE = "./assets/player.png";
constexpr auto ROOT_TEXTURE = "./assets/root.png";
constexpr auto ROOT_END_TEXTURE = "./assets/plat4.png";
constexpr auto BG_TEXTURE = "./assets/bg9.png";
constexpr auto AUDIO_MUSIC = "./assets/music.ogg";
constexpr auto AUDIO_HIT = "./assets/hit.wav";
constexpr auto AUDIO_MISS = "./assets/miss.wav";
constexpr auto ENEMY_TEXTURE = "./assets/Slime_sheet.png";
constexpr auto PARTICLE_TEXTURE = "./assets/particle.png";

constexpr int WIN_W = 1280;
constexpr int WIN_H = 720;
constexpr int WORLD_W = 2560;
constexpr int WORLD_H = 720;
constexpr int BG_SCALE = 6.f;
constexpr int BG_RELATIVE_SCALE = 13.f;

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
#include "Enemy.h"
#include "audio.h"
#include "menu.h"
#include "Explosion.h"
#include "game.h"
