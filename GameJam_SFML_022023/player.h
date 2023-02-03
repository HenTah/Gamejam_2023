#pragma once
#include "main.h"

constexpr int	PLAYER_SPRITESHEET_HORIZONTAL_COUNT		= 10;
constexpr int	PLAYER_SPRITESHEET_VERTICAL_COUNT		= 18;
constexpr int	PLAYER_SPRITESHEET_FACING_RIGHT_COUNT	= 90;

constexpr int	DEFAULT_HEALTH				= 1000;
constexpr int	PLAYER_W					= 400;
constexpr int	PLAYER_H					= 220;
constexpr float	PLAYER_CLAMP_PADDING		= 200.f;
constexpr float	MOVEMENT_ACCELERATION		= 10000.f;
constexpr float	MOVEMENT_DRAG_COEFFICIENT	= 0.9f;
constexpr float	JUMP_VELOCITY				= 2000.f;
constexpr float	MIN_HORIZONTAL_MOVEMENT		= 0.0001f;
constexpr float	MAX_HORIZONTAL_SPEED		= 1200.f;
constexpr float	MAX_VERTICAL_SPEED			= 50000.f;
constexpr float	GRAVITY						= 6000.f;
constexpr float	ANIMATION_UPDATE_FREQUENCY	= 0.1f;

constexpr std::pair<int, int> IDLE_FRAMES (0, 6);

enum e_animation_states
{
	IDLE,
	RUN,
};

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
	sf::Clock		_animation;
	int				_animation_state;
	int				_frame_id;
	bool			_facing_right;

	void			_clamp_position();
	void			_clamp_velocity();
	void			_jump();
	void			_progress_animation();
	void			_update_frame();
};
