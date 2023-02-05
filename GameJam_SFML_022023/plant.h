#pragma once
#include "main.h"
class Game;

class RootSegment : public sf::RectangleShape
{
public:
	RootSegment(sf::Vector2f position, float size, float speed, const sf::Texture* texture);
	void			update(float dt);

	bool			 get_active() const { return _active; }
	void			set_active(bool value) { _active = value; }
private:
	bool			_active;
	float			_target_y_size;
	float			_texture_y_scale;
	sf::Vector2f	_position;
	sf::Vector2f	_speed;
};

class Root
{
public:
	Root(sf::Vector2f position, float size, float speed, const Game* game);
	void			update(float dt);
	void			draw(sf::RenderWindow& window);
	void			translate(sf::Vector2f offset);
	float get_pos();
	sf::FloatRect	getGlobalBounds();
	int intersects(sf::FloatRect collider);
	void			cut(float height);
	bool			getState();
	void			cut(int i);
private:
	float			_speed;
	bool			_killed;
	bool			_grounded;
	sf::RectangleShape _base;
	std::vector<RootSegment> _segments;
	float			_alive;
	const sf::Texture* _tex;
	sf::RectangleShape _root_base;
};
