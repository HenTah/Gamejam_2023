#include "main.h"
#include <iostream>

	RootSegment::RootSegment(sf::Vector2f position, double size, const sf::Texture *texture)
	{
		this->_active = true;
		this->_target_y_size = size;
		this->_texture_y_scale = 0.f;
		this->_speed = sf::Vector2f(25.f, 25.f);
		this->setPosition(position.x - size * 0.64 / 2, position.y);
		this->setTexture(texture);
		this->setSize(sf::Vector2f(size * 0.64, 0.f));
		this->setTextureRect(sf::IntRect(0, 0, texture->getSize().x, 0.f));
		this->setOrigin(size * 0.64 / 2, 0.f);
	}
	void RootSegment::update(float dt)
	{
		if (get_active())
		{
			this->setSize(sf::Vector2f(this->getSize().x, this->getSize().y + _speed.y * dt));
			_texture_y_scale += dt * _speed.y * (400 / _target_y_size); //400 is texture y size
			this->setTextureRect(sf::IntRect(0, 0, this->getTextureRect().width, _texture_y_scale));
			if (this->getSize().y > _target_y_size)
			{
				this->setSize(sf::Vector2f(this->getSize().x, _target_y_size));
				this->setTextureRect(sf::IntRect(0, 0, this->getTextureRect().width, this->getTexture()->getSize().y));
				set_active(false);
			}
		}

		//this->setPosition(this->getPosition() + sf::Vector2f(0, this->speed.y * dt));
	}

	Root::Root(sf::Vector2f position, double size, const sf::Texture *texture)
	{
		_base.setPosition(position.x - size * 0.64 / 2, position.y);
		_base.setSize(sf::Vector2f(size * 0.64, size));
		_tex = texture;
		_alive = 0.f;
		_killed = false;
		_segments.emplace_back(_base.getPosition(), size, _tex);
		position.y += size;
	}
	void Root::update(float dt)
	{
		_alive += dt;
		if (!_segments.back().get_active() && _alive > 0.f)
		{
			_segments.emplace_back(sf::Vector2f(_base.getPosition().x, _base.getPosition().y + _base.getSize().y), _base.getSize().y, this->_tex);
			_base.setPosition(sf::Vector2f(_base.getPosition().x, _base.getPosition().y + _base.getSize().y));
		}
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
	}
	void Root::translate(sf::Vector2f offset)
	{
		for (auto& segment : _segments)
		{
			sf::Vector2f newPosition = segment.getPosition() + offset;
			segment.setPosition(newPosition);
		}
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

	void Root::cut(double height)
	{
		for (auto& segment : _segments)
		{
			if (height < segment.getPosition().y)
			{
				_base.setPosition(sf::Vector2f(_base.getPosition().x, _segments.back().getPosition().y - _base.getSize().y));
				_segments.pop_back();
				_alive = -2.5f;
				break;
			}
		}
	}
