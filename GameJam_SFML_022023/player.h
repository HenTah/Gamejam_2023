#pragma once
#include "main.h"

constexpr int	PLAYER_SPRITESHEET_HORIZONTAL_COUNT		= 10;
constexpr int	PLAYER_SPRITESHEET_VERTICAL_COUNT		= 18;
constexpr int	PLAYER_SPRITESHEET_FACING_RIGHT_COUNT	= 90;

constexpr int	DEFAULT_HEALTH				= 1000;
constexpr int	PLAYER_W					= 400;
constexpr int	PLAYER_H					= 220;
constexpr int	PLAYER_BOUNCE_HITBOX_H		= 10;
constexpr int	PLAYER_BOUNCE_HITBOX_W		= 100;
constexpr float	PLAYER_CLAMP_PADDING		= 200.f;
constexpr float	MOVEMENT_ACCELERATION		= 10000.f;
constexpr float	MOVEMENT_DRAG_COEFFICIENT	= 0.9f;
constexpr float	JUMP_VELOCITY				= 2000.f;
constexpr float	JUMP_BOUNCE_MULTIPLIER		= 1.22f;
constexpr float	MIN_HORIZONTAL_MOVEMENT		= 0.0001f;
constexpr float	MAX_HORIZONTAL_SPEED		= 1200.f;
constexpr float	MAX_VERTICAL_SPEED			= 50000.f;
constexpr float	GRAVITY						= 6000.f;
constexpr float	ANIMATION_UPDATE_FREQUENCY	= 0.1f;

constexpr float	ATTACK_BOUNDS_W = 160.f;
constexpr float	ATTACK_BOUNDS_H = 200.f;

enum e_animation_states
{
	IDLE,
	RUN,
	ATTACK,
	HIT,
	JUMP
};

constexpr std::pair<int, int> ANIMATION_FRAMES[5] = {
{0, 6},
{7, 17},
{30, 36},
{32, 33},
{37, 44}
};

class Enemy;

class Player : public sf::RectangleShape
{
public:
	Player();
	void			init(sf::Texture* texture);
	void			take_damage(int damage);
	void			handle_movement(sf::Time delta);
	void			update_position(sf::Time delta);
	void			attack();
	sf::FloatRect	get_attack_bounds();
	bool			is_attacking();
	bool			check_slime_bounce(Enemy& enemy);
private:
	int				_health;
	sf::Vector2f	_velocity;
	sf::Clock		_animation;
	int				_animation_state;
	bool			_is_idle;
	int				_frame_id;
	bool			_facing_right;

	void			_clamp_position();
	void			_clamp_velocity();
	void			_jump();
	void			_change_state(int state);
	void			_progress_animation();
	void			_update_frame();
};
