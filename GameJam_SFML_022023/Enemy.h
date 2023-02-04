#pragma once
#include "main.h"
constexpr int	ENEMY_SPRITESHEET_HORIZONTAL_COUNT = 5;
constexpr int	ENEMY_SPRITESHEET_VERTICAL_COUNT = 6;

class Enemy : public sf::RectangleShape
{
public:
	Enemy(sf::Vector2f pos, double scale, double speed, sf::Texture* texture);
	void			update_position(sf::Time delta, std::vector<Root>* roots);
private:
	bool			_walk;
	bool			_grounded;
	Root			*_climbing_root;
	sf::Clock		_animation;
	sf::Vector2f	_area;
	int				_health;
	sf::Vector2f	_velocity;
	int				_frame_id;
	void			_update_frame();
	void			_progress_animation();
};

