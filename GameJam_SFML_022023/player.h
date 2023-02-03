#pragma once
#include <SFML/Graphics.hpp>

constexpr int	DEFAULT_HEALTH = 1000;

class Player : public sf::RectangleShape
{
private:
	int				health;
	sf::Vector2f	pos;
public:
	Player();
	void	take_damage(int damage);
};
