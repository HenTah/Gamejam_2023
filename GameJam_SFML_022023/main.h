#pragma once

#define _USE_MATH_DEFINES

constexpr auto PLAYER_TEXTURE = "./assets/player.png";
constexpr auto ROOT_TEXTURE = "./assets/root.png";
constexpr auto GAME_NAME = "PLANT KILLER";

constexpr int WIN_W = 1280;
constexpr int WIN_H = 720;

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <chrono>
#include <cmath>
#include <random>
#include <iostream>
#include <random>
#include "plant.h"
#include "player.h"
#include "game.h"
#include "Explosion.h"
