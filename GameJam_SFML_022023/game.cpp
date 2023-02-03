#include "game.h"

Game::Game(const char *name, int w, int h) {
	window.create(sf::VideoMode(w, h), name);
	window.setVerticalSyncEnabled(true);
	delta = clock.restart();
	if (!texture.loadFromFile(ROOT_TEXTURE))
		exit(NULL);
	roots.emplace_back(2, sf::Vector2f(WIN_W / 3, 0), sf::Vector2f(50.f, 50.f), texture);
	if (!texture_player.loadFromFile(PLAYER_TEXTURE))
		exit(NULL);
	player.init(&texture_player);
}

void	Game::handle_events(sf::Event& event) {
	if (event.type == sf::Event::Closed)
		window.close();
}

void	Game::update_values() {	
	for (Root& root : roots)
	{
		root.update(delta.asSeconds());
	}
	player.handle_movement(delta);
	player.update_position(delta);
}

void	Game::render() {
	window.clear();

	for (Root& root : roots)
	{
		root.draw(window);
	}
	window.draw(player);
	window.display();
}