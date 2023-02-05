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
	std::vector<Enemy>	enemies;
	std::vector<Explosion> particles;
	sf::Clock			clock;
	sf::Time			delta;
	sf::RenderWindow	window;
	sf::View			view;
	sf::Texture			texture;
	sf::Texture			texture_player;
	Player				player;
	Audio				audio;
	Menu				menu;
	sf::Texture			bg_texture;
	sf::Sprite			bg_sprite;
	sf::Texture			texture_root_end;
	sf::Texture			enemy_texture;
	sf::Texture			particle_texture;

	Game();

	void	handle_events(sf::Event &event);
	void	update_growth(void);
	void	spawner(void);
	void	update_values(void);
	void	render();
	void	handle_view();
	void	set_state(int state);
	void	exit_game();

private:
	sf::Clock			_growth_timer;
	sf::Clock			_spawn_timer;
	int					_state;
	float				_next_growth;
	float				_next_spawn;

};