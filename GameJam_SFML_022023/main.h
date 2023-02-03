#pragma once

#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <chrono>
#include <cmath>
#include <random>
#include <iostream>
#include "plant.h"
#include "player.h"
#include "game.h"
#define ROOT_TEXTURE "./assets/root.png"

constexpr auto GAME_NAME = "PLANT KILLER";
constexpr int WIN_W = 800;
constexpr int WIN_H = 600;