#include "main.h"
#include <iostream>

RootSegment::RootSegment(sf::Vector2f position, float size, float speed, const sf::Texture* texture)
{
	this->_active = true;
	this->_target_y_size = size;
	this->_texture_y_scale = 0.f;
	this->_speed = sf::Vector2f(speed, speed);
	this->setPosition(position.x - size * 0.64f / 2.f, position.y);
	this->setTexture(texture);
	this->setSize(sf::Vector2f(size * 0.64f, 0.f));
	this->setTextureRect(sf::IntRect(0, 0, texture->getSize().x, 0));
	this->setOrigin(size * 0.64f / 2.f, 0.f);
}
void RootSegment::update(float dt)
{
	if (get_active())
	{
		this->setSize(sf::Vector2f(this->getSize().x, this->getSize().y + _speed.y * dt));
		_texture_y_scale += dt * _speed.y * (75 / _target_y_size); //75 is texture y size
		this->setTextureRect(sf::IntRect(0, 0, this->getTextureRect().width, (int)_texture_y_scale));
		if (this->getSize().y > _target_y_size)
		{
			this->setSize(sf::Vector2f(this->getSize().x, _target_y_size));
			this->setTextureRect(sf::IntRect(0, 0, this->getTextureRect().width, this->getTexture()->getSize().y));
			set_active(false);
		}
	}

	//this->setPosition(this->getPosition() + sf::Vector2f(0, this->speed.y * dt));
}

Root::Root(sf::Vector2f position, float size, float speed, const Game* game)
{
	_speed = speed;
	_base.setPosition(position.x - size * 0.64f / 2.f, position.y);
	_base.setSize(sf::Vector2f(size * 0.64f, size));
	_tex = &(game->root_texture);
	_root_base.setOrigin(size / 2.f, 0.f);
	_root_base.setTexture(&(game->texture_root_end));
	_root_base.setSize(sf::Vector2f(size * 0.8f, size));
	_root_base.setPosition(position - sf::Vector2f(_root_base.getSize().x * 0.64f, _root_base.getSize().y / 4));

	_alive = 0.f;
	_grounded = false;
	_killed = false;
	_segments.emplace_back(_base.getPosition(), size, _speed, _tex);
	position.y += size;
}
void Root::update(float dt)
{
	if (_grounded || _segments.empty())
		return;
	_alive += dt;
	if (_alive > 0.f && !_segments.back().get_active())
	{
		_segments.emplace_back(sf::Vector2f(_base.getPosition().x, _base.getPosition().y + _base.getSize().y), _base.getSize().y, _speed, this->_tex);
		_base.setPosition(sf::Vector2f(_base.getPosition().x, _base.getPosition().y + _base.getSize().y));
	}
	_root_base.setPosition(_segments.back().getPosition() + _segments.back().getSize() - sf::Vector2f(_root_base.getSize().x * 0.64f, _root_base.getSize().y / 4));
	if (_segments.back().getPosition().y > WIN_H - 50.f)
		_grounded = true;
	for (auto& segment : _segments)
	{
		segment.update(dt);
	}
}
void Root::draw(sf::RenderWindow& window)
{
	for (auto& segment : _segments)
	{
		window.draw(segment);
	}
	window.draw(_root_base);
}
void Root::translate(sf::Vector2f offset)
{
	for (auto& segment : _segments)
	{
		sf::Vector2f newPosition = segment.getPosition() + offset;
		segment.setPosition(newPosition);
	}
}
float Root::get_pos()
{
	return _base.getPosition().x;
}

sf::FloatRect Root::getGlobalBounds()
{
	bool first = true;
	sf::FloatRect rect;
	for (auto& segment : _segments)
	{
		sf::FloatRect root = segment.getGlobalBounds();

		if (first)
		{
			rect = root;
			first = false;
			continue;
		}
		rect.left = std::min(rect.left, root.left);
		rect.top = std::min(rect.top, root.top);
		rect.width = std::max(rect.left + rect.width, root.left + root.width) - rect.left;
		rect.height = std::max(rect.top + rect.height, root.top + root.height) - rect.top;
	}
	return rect;

}

int Root::intersects(sf::FloatRect collider)
{
	sf::FloatRect rect;
	for (size_t i = 0; i < _segments.size(); i++)
	{
		sf::FloatRect root_segment = _segments[i].getGlobalBounds();
		if (root_segment.intersects(collider))
		{
			return i;
		}

	}
	return -1;
}

void Root::cut(float height)
{
	for (const auto& segment : _segments)
	{
		if (height < segment.getPosition().y + segment.getSize().y)
		{
			_base.setPosition(sf::Vector2f(_base.getPosition().x, _segments.back().getPosition().y - _base.getSize().y));
			_segments.pop_back();
			_alive = -10.f;
			_grounded = false;
			break;
		}
	}
}

bool Root::getState()
{
	return _grounded;
}

void Root::cut(int i)
{
	if (i < 0)
		return;
	auto it = (_segments.begin() + i);
	_segments.erase(it, _segments.end());
	_base.setPosition(sf::Vector2f(_base.getPosition().x, _segments.back().getPosition().y));
	_alive = -15.f;
	_grounded = false;
}
