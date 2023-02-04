#pragma once
#include "main.h"

constexpr auto UI_TEXTURE = "./assets/ui.png";

class Game;

class Menu
{
public:
	Menu();
	void	init();
	void	render(sf::RenderWindow &window);
	void	update_values(Game *game);
	void	handle_events(sf::Event& event, Game* game);
private:
	sf::Texture			_ui_texture;
	sf::RectangleShape	_volume_bar_frame;
	sf::RectangleShape	_volume_bar;
	sf::RectangleShape	_button_play;
	sf::RectangleShape	_button_exit;
	sf::RectangleShape	_bg_shadow;
};
