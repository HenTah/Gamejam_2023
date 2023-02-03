#include "main.h"
#include <iostream>

	RootSegment::RootSegment(sf::Vector2f position, double size, const sf::Texture *texture)
	{
		this->active = true;
		this->target_y_size = size;
		this->texture_y_scale = 0.f;
		this->speed = sf::Vector2f(25.f, 25.f);
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
			this->setSize(sf::Vector2f(this->getSize().x, this->getSize().y + speed.y * dt));
			texture_y_scale += dt * speed.y * (400 / target_y_size); //400 is texture y size
			this->setTextureRect(sf::IntRect(0, 0, this->getTextureRect().width, texture_y_scale));
			if (this->getSize().y > target_y_size)
			{
				this->setSize(sf::Vector2f(this->getSize().x, target_y_size));
				this->setTextureRect(sf::IntRect(0, 0, this->getTextureRect().width, this->getTexture()->getSize().y));
				set_active(false);
			}
		}

		//this->setPosition(this->getPosition() + sf::Vector2f(0, this->speed.y * dt));
	}

	Root::Root(sf::Vector2f position, double size, const sf::Texture *texture)
	{
		base.setPosition(position.x - size * 0.64 / 2, position.y);
		base.setSize(sf::Vector2f(size * 0.64, size));
		tex = texture;
		alive = 0.f;
		killed = false;
		segments.emplace_back(base.getPosition(), size, tex);
		position.y += size;
	}
	void Root::update(float dt)
	{
		alive += dt;
		if (!segments.back().get_active() && alive > 0.f)
		{
			segments.emplace_back(sf::Vector2f(base.getPosition().x, base.getPosition().y + base.getSize().y), base.getSize().y, this->tex);
			base.setPosition(sf::Vector2f(base.getPosition().x, base.getPosition().y + base.getSize().y));
		}
		for (auto& segment : segments)
		{
			segment.update(dt);
		}
	}
	void Root::draw(sf::RenderWindow& window)
	{
		for (auto& segment : segments)
		{
			window.draw(segment);
		}
	}
	void Root::translate(sf::Vector2f offset)
	{
		for (auto& segment : segments)
		{
			sf::Vector2f newPosition = segment.getPosition() + offset;
			segment.setPosition(newPosition);
		}
	}

	sf::FloatRect Root::getGlobalBounds()
	{
		bool first = true;
		sf::FloatRect rect;
		for (auto& segment : segments)
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
		for (auto& segment : segments)
		{
			if (height < segment.getPosition().y)
			{
				base.setPosition(sf::Vector2f(base.getPosition().x, segments.back().getPosition().y - base.getSize().y));
				segments.pop_back();
				alive = -2.5f;
				break;
			}
		}
	}
