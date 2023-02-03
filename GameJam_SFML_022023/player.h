#pragma once
#include "main.h"

constexpr int	DEFAULT_HEALTH	= 1000;
constexpr int	PLAYER_W		= 400;
constexpr int	PLAYER_H		= 220;

class Player : public sf::RectangleShape
{
public:
	Player();
	void	init(sf::Texture* texture);
	void	take_damage(int damage);
private:
	int				health;
	sf::Vector2f	pos;
};
