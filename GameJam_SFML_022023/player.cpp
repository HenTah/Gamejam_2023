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
	this->_frame_id = 0;
	this->_facing_right = true;
}

void	Player::take_damage(int damage)
{
	this->_health -= damage;
	printf("HP: %d\n", this->_health);
}

void	Player::handle_movement(sf::Time delta)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		this->_facing_right = false;
		this->_velocity += sf::Vector2f(-MOVEMENT_ACCELERATION * delta.asSeconds(), 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		this->_facing_right = true;
		this->_velocity += sf::Vector2f(MOVEMENT_ACCELERATION * delta.asSeconds(), 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		this->_jump();

	this->_velocity.x *= MOVEMENT_DRAG_COEFFICIENT;
	if (fabsf(this->_velocity.x) < MIN_HORIZONTAL_MOVEMENT)
		this->_velocity.x = 0.f;
	this->_velocity.y += GRAVITY * delta.asSeconds();
	this->_clamp_velocity();
	this->_animation_state = IDLE;
	if (this->_animation.getElapsedTime().asSeconds() > ANIMATION_UPDATE_FREQUENCY)
		this->_progress_animation();
}

void	Player::update_position(sf::Time delta)
{
	this->move(this->_velocity * delta.asSeconds());
	this->_clamp_position();
}

void	Player::_jump()
{
	if (this->getPosition().y < 0.f)
		return;
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
	if (this->_animation_state == IDLE && this->_frame_id > IDLE_FRAMES.second)
		this->_frame_id = IDLE_FRAMES.first;
	this->_update_frame();
}
