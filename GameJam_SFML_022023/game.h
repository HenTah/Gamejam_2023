#pragma once
#include <SFML/Graphics.hpp>
#include "player.h"

enum e_game_status
{
	STATUS_GAME,
	STATUS_GAMEOVER
};

class Game {
public:
	int					status;
	Player				player;
	//std::vector<Plant>	plants;
	sf::Clock			clock;
	sf::Time			delta;
	sf::RenderWindow	window;

	Game(const char* name, int w, int h);

	void	handle_events(sf::Event &event);
	void	update_values(void);
	void	render();
};