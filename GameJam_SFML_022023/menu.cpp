#include "main.h"

Menu::Menu()
{
}

void	Menu::init(Game* game)
{
	if (!_ui_texture.loadFromFile(UI_TEXTURE))
		exit(NULL);

	_bg_shadow.setSize(sf::Vector2f((float)(WIN_W), (float)(WIN_H)));
	_bg_shadow.setFillColor(sf::Color(0, 0, 0, 200));

	_button_play.setTexture(&_ui_texture);
	_button_play.setSize(sf::Vector2f(486.f, 150.f));
	_button_play.setTextureRect(sf::IntRect(0, 150, 486, 150));
	_button_play.setScale(1.f, 1.f);
	_button_play.setPosition(sf::Vector2f((float)((WIN_W - 486) / 2), (float)(WIN_H / 4 * 1 - 150 / 2)));

	_button_exit.setTexture(&_ui_texture);
	_button_exit.setSize(sf::Vector2f(344.f, 173.f));
	_button_exit.setTextureRect(sf::IntRect(1070, 128, 344, 173));
	_button_exit.setScale(1.f, 1.f);
	_button_exit.setPosition(sf::Vector2f((float)((WIN_W - 344) / 2), (float)(WIN_H / 4 * 2 - 173 / 2)));

	_volume_bar_frame.setTexture(&_ui_texture);
	_volume_bar_frame.setSize(sf::Vector2f(395.f, 37.f));
	_volume_bar_frame.setTextureRect(sf::IntRect(569, 237, 395, 37));
	_volume_bar_frame.setScale(1.f, 1.f);
	_volume_bar_frame.setPosition(sf::Vector2f((float)((WIN_W - 365) / 2), (float)(WIN_H / 4 * 3 - 37 / 2)));

	_volume_bar.setTexture(&_ui_texture);
	_volume_bar.setSize(sf::Vector2f(DEFAULT_VOLUME * 395 / 100.f, 37.f));
	_volume_bar.setTextureRect(sf::IntRect(569, 173, DEFAULT_VOLUME * 395 / 100, 37));
	_volume_bar.setScale(1.f, 1.f);
	_volume_bar.setPosition(sf::Vector2f((float)((WIN_W - 365) / 2), (float)(WIN_H / 4 * 3 - 37 / 2)));

	_text.setFont(game->font);
	_text.setString("Volume");
	_text.setCharacterSize(32);
	_text.setFillColor(sf::Color::White);
	_text.setPosition(sf::Vector2f((float)((WIN_W - 70) / 2), (float)(WIN_H / 4 * 3 - 60)));
}

void	Menu::update_values(Game* game)
{
	_volume_bar.setSize(sf::Vector2f(game->audio.get_volume() * 395 / 100.f, 37.f));
	_volume_bar.setTextureRect(sf::IntRect(569, 173, game->audio.get_volume() * 395 / 100, 37));
}

void	Menu::render(sf::RenderWindow& window)
{
	float	offset = -(window.getView().getCenter().x - WIN_W / 2.f);

	_bg_shadow.setOrigin(sf::Vector2f(offset, 0.f));
	_button_play.setOrigin(sf::Vector2f(offset, 0.f));
	_button_exit.setOrigin(sf::Vector2f(offset, 0.f));
	_volume_bar_frame.setOrigin(sf::Vector2f(offset, 0.f));
	_volume_bar.setOrigin(sf::Vector2f(offset, 0.f));
	_text.setOrigin(sf::Vector2f(offset, 0.f));
	window.draw(_bg_shadow);
	window.draw(_button_play);
	window.draw(_button_exit);
	window.draw(_volume_bar_frame);
	window.draw(_volume_bar);
	window.draw(_text);
}

void	Menu::handle_actions(Game* game)
{
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		return;
	
	sf::Vector2f	pos = sf::Vector2f(
		(float)sf::Mouse::getPosition(game->window).x,
		(float)sf::Mouse::getPosition(game->window).y);
	float			offset = -(game->window.getView().getCenter().x - WIN_W / 2.f);

	pos.x -= offset;

	if (_button_play.getGlobalBounds().contains(pos))
		game->set_state(STATE_GAME);

	if (_button_exit.getGlobalBounds().contains(pos))
		game->exit_game();

	if (_volume_bar_frame.getGlobalBounds().contains(pos))
		game->audio.set_volume((int)((pos.x - _volume_bar_frame.getGlobalBounds().left)
				/ _volume_bar_frame.getGlobalBounds().width * 100.f));
}