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
	sf::Texture			root_texture;
	sf::Texture			player_texture;
	sf::Texture			ui_texture;
	sf::Font			font;
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
	sf::RectangleShape	_overwhelming_bar_frame;
	sf::RectangleShape	_overwhelming_bar;
	sf::Text			_text;
	int					_grounded_enemies;

	void				_init_ui();
	void				_update_ui();
	void				_draw_ui();
};