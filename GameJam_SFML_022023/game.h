#pragma once
#include "main.h"

enum e_game_status
{
	STATE_GAME,
	STATE_MENU
};

class Game {
public:
	std::vector<Root>	roots;
	sf::Clock			clock;
	sf::Time			delta;
	sf::Clock			growth_timer;
	sf::RenderWindow	window;
	sf::Texture			texture;
	sf::Texture			texture_player;
	Player				player;
	Audio				audio;
	Menu				menu;
	sf::Texture			bg_texture;
	sf::Sprite			bg_sprite;

	Game(const char* name, int w, int h);

	void	handle_events(sf::Event &event);
	void	update_growth(void);
	void	update_values(void);
	void	render();
	void	set_state(int state);

private:
	int					_state;
	float				_growth_value;

};