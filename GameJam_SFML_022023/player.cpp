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
	this->setOrigin((float)(-(WIN_W - PLAYER_W) / 2), (float)(-WIN_H + PLAYER_H));
	this->setScale(1.f, 1.f);
	this->setPosition(sf::Vector2f(0.f, 0.f));
}

void	Player::take_damage(int damage) {
	this->health -= damage;
	printf("HP: %d\n", this->health);
}
