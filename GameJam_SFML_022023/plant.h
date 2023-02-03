#pragma once
#include "main.h"

class RootSegment : public sf::RectangleShape
{
public:
	RootSegment(sf::Vector2f position, sf::Vector2f size, const sf::Texture& texture);
	void update(float dt);

private:
	sf::Vector2f position;
	sf::Vector2f speed;
};

class Root
{
public:
	Root(int numSegments, sf::Vector2f position, sf::Vector2f size, const sf::Texture& texture);
	void update(float dt);
	void draw(sf::RenderWindow& window);
	void translate(sf::Vector2f offset);
	void cut(double height);
private:
	std::vector<RootSegment> segments;
};
