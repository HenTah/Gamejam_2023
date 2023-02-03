#include "main.h"

Player::Player() {
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
		this->_velocity += sf::Vector2f(-MOVEMENT_ACCELERATION * delta.asSeconds(), 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->_velocity += sf::Vector2f(MOVEMENT_ACCELERATION * delta.asSeconds(), 0.f);
	this->_velocity.x *= MOVEMENT_DRAG_COEFFICIENT;
	if (fabsf(this->_velocity.x) < MIN_HORIZONTAL_MOVEMENT)
		this->_velocity.x = 0.f;
}

void	Player::update_position(sf::Time delta)
{
	this->move(this->_velocity * delta.asSeconds());
	
	this->_clamp_position();
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