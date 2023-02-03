#pragma once
#include "main.h"

constexpr int	DEFAULT_HEALTH				= 1000;
constexpr int	PLAYER_W					= 400;
constexpr int	PLAYER_H					= 220;
constexpr float	PLAYER_CLAMP_PADDING		= 200.f;
constexpr float	MOVEMENT_ACCELERATION		= 10000.f;
constexpr float	MOVEMENT_DRAG_COEFFICIENT	= 0.9f;
constexpr float	MIN_HORIZONTAL_MOVEMENT		= 0.0001f;
constexpr float	MAX_MOVEMENT_SPEED			= 1200.f;

class Player : public sf::RectangleShape
{
public:
	Player();
	void	init(sf::Texture* texture);
	void	take_damage(int damage);
	void	handle_movement(sf::Time delta);
	void	update_position(sf::Time delta);
private:
	int				_health;
	sf::Vector2f	_velocity;
	void			_clamp_position();
};
