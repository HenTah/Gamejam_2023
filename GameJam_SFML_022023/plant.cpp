#include "main.h"
#include <iostream>

	RootSegment::RootSegment(sf::Vector2f position, sf::Vector2f size, const sf::Texture& texture)
	{
		this->speed = sf::Vector2f(50.f, 50.f);
		this->setPosition(position.x - size.x / 2, position.y);
		this->setTexture(&texture);
		this->setFillColor(sf::Color::Green);
		this->setSize(sf::Vector2f(size.x, size.y));
		this->setOrigin(size.x / 2, 0.f);
	}
	void RootSegment::update(float dt)
	{
		this->setPosition(this->getPosition() + sf::Vector2f(0, this->speed.y * dt));
	}

	Root::Root(int numSegments, sf::Vector2f position, sf::Vector2f size, const sf::Texture& texture)
	{
		for (int i = 0; i < numSegments; ++i) {
			segments.emplace_back(position, size, texture);
			position.y += size.y;
		}
	}
	void Root::update(float dt)
	{
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
	void Root::cut(double height)
	{
		
	}
