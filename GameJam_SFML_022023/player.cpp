#include "player.h"

Player::Player() {
	this->health = DEFAULT_HEALTH;
	this->setSize(sf::Vector2f(1.f, 1.f));
}

void	Player::take_damage(int damage) {
	this->health -= damage;
	printf("HP: %d\n", this->health);
}
