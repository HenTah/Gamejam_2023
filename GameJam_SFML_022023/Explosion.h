#pragma once
#include "main.h"

class Explosion {
public:
	Explosion(sf::Vector2f position, sf::Color color, int particle_count, sf::Texture& texture);
	void Update(float dt);
	void Draw(sf::RenderWindow& window);
	double get_alive();
private:
	sf::Clock	_alive;
	std::vector<sf::Sprite> particles_;
	std::vector<sf::Vector2f> velocities_;
	sf::Vector2f position_;
	sf::Color color_;
};

