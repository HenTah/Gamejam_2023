#include "main.h"

Enemy::Enemy(sf::Vector2f pos, double scale, sf::Texture* texture)
{
	_health = 100.f;
	_walk = true;
	setTexture(texture);
	setSize(sf::Vector2f(64.f * scale, 44.f * scale));
	setTextureRect(sf::IntRect(0, 0, 320 / 5, 261 /6 ));
	_velocity = sf::Vector2f(100.f, 0.f);
	_frame_id = 0;

	setPosition(sf::Vector2f(WIN_W / 2, WIN_H / 2));
}

void	Enemy::update_position(sf::Time delta, std::vector<Root> *roots)
{
	for (Root& root : *roots)
	{
		if (root.getState())
		{
			sf::FloatRect rootbounds = root.getGlobalBounds();
			sf::FloatRect bounds = this->getGlobalBounds();

		}
	}

	if (_walk)
	{
		_velocity.y = 0.f;
		if (getPosition().x > WIN_W - getSize().x)
			_velocity.x = -_velocity.x;
		if (getPosition().x < 0)
			_velocity.x = -_velocity.x;
	}
	else
	{
		_velocity.y = 100.f;
	}
	this->move(this->_velocity * delta.asSeconds());
	if (this->_animation.getElapsedTime().asSeconds() > ANIMATION_UPDATE_FREQUENCY)
		_progress_animation();
}

void	Enemy::_update_frame()
{
	int		frame = this->_frame_id;
	int		x;
	int		y;

	x = frame % ENEMY_SPRITESHEET_HORIZONTAL_COUNT;
	y = frame / ENEMY_SPRITESHEET_HORIZONTAL_COUNT;

	this->setTextureRect(sf::IntRect(
		x * 64,
		y * 43.5,
		64,
		44));
}

void	Enemy::_progress_animation()
{
	this->_animation.restart();
	this->_frame_id++;
	if (this->_frame_id >= 5 * 6)
	{
			this->_frame_id = 0;
	}
	this->_update_frame();
}