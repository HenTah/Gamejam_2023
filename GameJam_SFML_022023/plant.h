#pragma once
#include "main.h"
class Game;

class RootSegment : public sf::RectangleShape
{
public:
	RootSegment(sf::Vector2f position, double size, double speed, const sf::Texture *texture);
	void			update(float dt);

	bool			 get_active() const { return _active; }
	void			set_active(bool value) { _active = value; }
private:
	bool			_active;
	double			_target_y_size;
	double			_texture_y_scale;
	sf::Vector2f	_position;
	sf::Vector2f	_speed;
};

class Root
{
public:
	Root(sf::Vector2f position, double size, double speed, const Game *game);
	void			update(float dt);
	void			draw(sf::RenderWindow& window);
	void			translate(sf::Vector2f offset);
	sf::FloatRect	getGlobalBounds();
	int intersects(sf::FloatRect collider);
	void			cut(double height);
	bool			getState();
	void			cut(int i);
private:
	double			_speed;
	bool			_killed;
	bool			_grounded;
	sf::RectangleShape _base;
	std::vector<RootSegment> _segments;
	double			_alive;
	const sf::Texture *_tex;
	sf::RectangleShape _root_base;
};
