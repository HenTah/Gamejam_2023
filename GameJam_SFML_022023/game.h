#pragma once
#include "main.h"

enum e_game_status
{
	STATUS_GAME,
	STATUS_GAMEOVER
};

class Game {
public:
	int					status;
	std::vector<Root>	roots;
	sf::Clock			clock;
	sf::Time			delta;
	sf::RenderWindow	window;
	sf::Texture			texture;
	sf::Texture			texture_player;
	Player				player;


	Game(const char* name, int w, int h);

	void	handle_events(sf::Event &event);
	void	update_values(void);
	void	render();
};