#include "main.h"

Player::Player()
{
}

void	Player::init(sf::Texture* texture)
{
	if (!texture)
		exit(NULL);
	this->setTexture(texture);
	this->setSize(sf::Vector2f((float)PLAYER_W, (float)PLAYER_H));
	this->setTextureRect(sf::IntRect(0, 0, PLAYER_W, PLAYER_H));
	this->setOrigin(0.f, (float)(-WIN_H + PLAYER_H));
	this->setScale(1.f, 1.f);
	this->setPosition(sf::Vector2f((float)((WIN_W - PLAYER_W) / 2), 0.f));
	this->_animation.restart();
	this->_animation_state = IDLE;
	this->_frame_id = 0;
	this->_facing_right = true;
}

void	Player::take_damage(int damage)
{
	this->_health -= damage;
}

void	Player::handle_movement(sf::Time delta)
{
	this->_is_idle = true;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		this->_facing_right = false;
		this->_velocity += sf::Vector2f(-MOVEMENT_ACCELERATION * delta.asSeconds(), 0.f);
		this->_change_state(RUN);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		this->_facing_right = true;
		this->_velocity += sf::Vector2f(MOVEMENT_ACCELERATION * delta.asSeconds(), 0.f);
		this->_change_state(RUN);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
		this->attack();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		this->_jump();

	if (this->_is_idle
		&& this->getPosition().y >= 0.f
		&& this->_animation_state != ATTACK)
		this->_change_state(IDLE);

	this->_velocity.x *= MOVEMENT_DRAG_COEFFICIENT;
	if (fabsf(this->_velocity.x) < MIN_HORIZONTAL_MOVEMENT)
		this->_velocity.x = 0.f;
	this->_velocity.y += GRAVITY * delta.asSeconds();
	this->_clamp_velocity();

	if (this->_animation.getElapsedTime().asSeconds() > ANIMATION_UPDATE_FREQUENCY)
		this->_progress_animation();
}

void	Player::update_position(sf::Time delta)
{
	this->move(this->_velocity * delta.asSeconds());
	this->_clamp_position();
}

void	Player::attack()
{
	this->_change_state(ATTACK);
}

sf::FloatRect	Player::get_attack_bounds()
{
	sf::FloatRect	bounds = this->getGlobalBounds();

	bounds.width = ATTACK_BOUNDS_W;
	bounds.height = ATTACK_BOUNDS_H;
	bounds.top += 10;
	if (this->_facing_right)
		bounds.left += PLAYER_W - ATTACK_BOUNDS_W;
	return (bounds);
}

bool	Player::is_attacking()
{
	return (this->_animation_state == ATTACK
		&& this->_frame_id >= ANIMATION_FRAMES[HIT].first
		&& this->_frame_id <= ANIMATION_FRAMES[HIT].second);
}

void	Player::_jump()
{
	if (this->getPosition().y < 0.f)
		return;
	this->_change_state(JUMP);
	this->_velocity.y = -JUMP_VELOCITY;
}

void	Player::_clamp_position()
{
	sf::Vector2f	pos = this->getPosition();

	if (pos.x < -PLAYER_CLAMP_PADDING)
		pos.x = -PLAYER_CLAMP_PADDING;
	if (pos.x > PLAYER_CLAMP_PADDING + (float)(WIN_W - PLAYER_W))
		pos.x = PLAYER_CLAMP_PADDING + (float)(WIN_W - PLAYER_W);
	if (pos.y > 0)
		pos.y = 0;
	if (pos.y < -(WIN_H - PLAYER_H))
		pos.y = -(WIN_H - PLAYER_H);
	if (pos.x != this->getPosition().x)
		this->_velocity.x = 0.f;
	this->setPosition(pos);
}

void	Player::_clamp_velocity()
{
	if (this->_velocity.x < -MAX_HORIZONTAL_SPEED)
		this->_velocity.x = -MAX_HORIZONTAL_SPEED;
	if (this->_velocity.x > MAX_HORIZONTAL_SPEED)
		this->_velocity.x = MAX_HORIZONTAL_SPEED;
	if (this->_velocity.y < -MAX_VERTICAL_SPEED)
		this->_velocity.y = -MAX_VERTICAL_SPEED;
	if (this->_velocity.y > MAX_VERTICAL_SPEED)
		this->_velocity.y = MAX_VERTICAL_SPEED;
}

void	Player::_change_state(int state)
{
	if (state == RUN && this->getPosition().y < 0.f)
		return;
	if (state != IDLE)
		this->_is_idle = false;
	if (this->_animation_state == state
		|| this->_animation_state == ATTACK)
		return;
	this->_animation_state = state;
	this->_frame_id = ANIMATION_FRAMES[this->_animation_state].first;
}

void	Player::_update_frame()
{
	int		frame = this->_frame_id;
	int		x;
	int		y;

	if (!this->_facing_right)
		frame += PLAYER_SPRITESHEET_FACING_RIGHT_COUNT;

	x = frame % PLAYER_SPRITESHEET_HORIZONTAL_COUNT;
	y = frame / PLAYER_SPRITESHEET_HORIZONTAL_COUNT;
	this->setTextureRect(sf::IntRect(
		x * PLAYER_W,
		y * PLAYER_H,
		PLAYER_W,
		PLAYER_H));
}

void	Player::_progress_animation()
{
	this->_animation.restart();
	this->_frame_id++;
	if (this->_frame_id > ANIMATION_FRAMES[this->_animation_state].second)
	{
		if (this->_animation_state == ATTACK)
		{
			this->_animation_state = IDLE;
			this->_change_state(IDLE);
		}
		else
			this->_frame_id = ANIMATION_FRAMES[this->_animation_state].first;
	}
	this->_update_frame();
}
