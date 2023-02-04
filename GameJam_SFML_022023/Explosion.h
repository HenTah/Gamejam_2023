#pragma once
#include "main.h"

class Explosion {
public:
	Explosion(sf::Vector2f position, sf::Color color, int particle_count);
	void Update(float dt);
	void Draw(sf::RenderWindow& window);

private:
	std::vector<sf::RectangleShape> particles_;
	std::vector<sf::Vector2f> velocities_;
	sf::Vector2f position_;
	sf::Color color_;
};

