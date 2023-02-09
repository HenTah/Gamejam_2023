#include "main.h"

Enemy::Enemy(sf::Vector2f pos, float scale, float speed, sf::Texture* texture)
{
	_health = 100.f;
	_walk = true;
	_grounded = false;
	setTexture(texture);
	setOrigin(sf::Vector2f(32.f, 44.f));
	setSize(sf::Vector2f(64.f * scale, 44.f * scale));
	setTextureRect(sf::IntRect(0, 0, 320 / 5, 261 / 6));
	_area = sf::Vector2f(0, WORLD_W);
	_velocity = sf::Vector2f(speed, 0.f);
	_frame_id = 0;
	_climbing_root = NULL;
	setPosition(sf::Vector2f(pos.x, pos.y + 50.f));
}

void	Enemy::hit()
{
	if (_health == 100.f)
		_health -= 1.f;
}

float	Enemy::get_health()
{
	return _health;
}

Root* Enemy::get_root()
{
	return _climbing_root;
}

void	Enemy::set_root()
{
	this->_climbing_root = NULL;
}

void	Enemy::update_position(sf::Time delta, std::vector<Root>* roots)
{
	sf::FloatRect bounds = this->getGlobalBounds();
	//looking for roots
	if (!this->_grounded && _walk)
	{
		for (auto& root : *roots)
		{
			if (root.getState())
			{
				sf::FloatRect rootbounds = root.getGlobalBounds();
				if (rootbounds.width > bounds.width && std::abs(rootbounds.left - bounds.left) < 50.f && bounds.intersects(rootbounds))
				{
					this->_climbing_root = &root;
					_area = sf::Vector2f(rootbounds.left, rootbounds.left + rootbounds.width);
					this->_walk = false;
					_velocity.y = 100.f;
					break;
				}
			}
		}
	}
	//if on ground set grounded and start walking
	if (!this->_grounded && bounds.top > WIN_H - bounds.height)
	{
		_velocity.y = 0.f;
		_area = sf::Vector2f(0, WORLD_W);
		this->_grounded = true;
		this->_climbing_root = NULL;
		//_velocity.x = 100.f;
		this->_walk = true;
	}
	//walk aroound game area
	if (this->_walk || !this->_grounded)
	{
		if (!this->_grounded && !this->_walk)
		{
			if (!this->_climbing_root)
				_velocity.y = 700.f;
		}
		if (getPosition().x > _area.y)
			_velocity.x = -std::abs(_velocity.x);
		if (getPosition().x < _area.x)
			_velocity.x = std::abs(_velocity.x);
	}
	this->move(this->_velocity * delta.asSeconds());
	if (_health < 100.f)
	{
		sf::Color color = this->getFillColor();
		if (128.f * delta.asSeconds() > color.a)
			color.a = 0;
		else
			color.a -= (sf::Uint8)(128.f * delta.asSeconds());
		this->setScale(sf::Vector2f(this->getScale().x + 1.2f * delta.asSeconds(), this->getScale().y - 0.8f * delta.asSeconds()));
		this->setFillColor(color);
		_health -= 10 * delta.asSeconds();
	}
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
		(int)(y * 43.5),
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

bool	Enemy::is_grounded()
{
	return _grounded;
}