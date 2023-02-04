#include "main.h"

Enemy::Enemy(sf::Vector2f pos, double scale, double speed, sf::Texture* texture)
{
	_health = 100.f;
	_walk = true;
	_grounded = false;
	setTexture(texture);
	setSize(sf::Vector2f(64.f * scale, 44.f * scale));
	setTextureRect(sf::IntRect(0, 0, 320 / 5, 261 /6 ));
	_area = sf::Vector2f(0, WIN_W);
	_velocity = sf::Vector2f(speed, 0.f);
	_frame_id = 0;
	_climbing_root = NULL;

	setPosition(sf::Vector2f(pos.x, pos.y + getSize().y / 2));
}

void	Enemy::update_position(sf::Time delta, std::vector<Root> *roots)
{
	sf::FloatRect bounds = this->getGlobalBounds();
	if (!_grounded && _walk)
	{
		for (Root& root : *roots)
		{
			if (root.getState())
			{
				sf::FloatRect rootbounds = root.getGlobalBounds();
				if (rootbounds.width > bounds.width && std::abs(rootbounds.left - bounds.left) < 50.f && bounds.intersects(rootbounds))
				{
					_climbing_root = &root;
					_area = sf::Vector2f(rootbounds.left, rootbounds.left + rootbounds.width);
					_walk = false;
					_velocity.y = 100.f;
					_velocity.x = 25.f;

				}
			}
		}
	}
	if (!_grounded && bounds.top > WIN_H - bounds.height)
	{
		_velocity.y = 0.f;
		_area = sf::Vector2f(0, WIN_W);
		_grounded = true;
		_velocity.x = 100.f;
		_walk = true;
	}
	if (_walk || !_grounded)
	{
		if (!_grounded && _climbing_root && !(_climbing_root->getState()))
		{
			_velocity.y = 500.f;
		}
		if (getPosition().x > _area.y - getSize().x)
			_velocity.x = -std::abs(_velocity.x);
		if (getPosition().x < _area.x)
			_velocity.x = std::abs(_velocity.x);
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