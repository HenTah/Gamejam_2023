#pragma once
#include "main.h"

class Plant : public sf::RectangleShape
{
public:
	Plant(sf::Texture* tex, double size, double pos);

	void grow(double speed, double time);
};

