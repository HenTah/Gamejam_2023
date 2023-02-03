#pragma once
#include "main.h"

class RootSegment : public sf::RectangleShape
{
public:
	RootSegment(sf::Vector2f position, double size, const sf::Texture *texture);
	void			update(float dt);

	bool			 get_active() const { return active; }
	void			set_active(bool value) { active = value; }
private:
	bool			active;
	double			target_y_size;
	double			texture_y_scale;
	sf::Vector2f	position;
	sf::Vector2f	speed;
};

class Root
{
public:
	Root(sf::Vector2f position, double size, const sf::Texture *texture);
	void			update(float dt);
	void			draw(sf::RenderWindow& window);
	void			translate(sf::Vector2f offset);
	sf::FloatRect	getGlobalBounds();
	void			cut(double height);
private:
	bool			killed;
	sf::RectangleShape base;
	std::vector<RootSegment> segments;
	double			alive;
	const sf::Texture		*tex;

};
