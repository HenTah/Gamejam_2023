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
	sf::Clock			growth_timer;
	sf::RenderWindow	window;
	sf::Texture			texture;
	sf::Texture			texture_player;
	Player				player;
	sf::Texture			bg_texture;
	sf::Sprite			bg_sprite;

	Game(const char* name, int w, int h);

	void	handle_events(sf::Event &event);
	void	update_growth(void);
	void	update_values(void);
	void	render();

private:
	float				_growth_value;

};