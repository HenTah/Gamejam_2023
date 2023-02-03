#include "game.h"

Game::Game(const char* name, int w, int h) {
	window.create(sf::VideoMode(w, h), name);
	window.setVerticalSyncEnabled(true);
	delta = clock.restart();
	if (!texture.loadFromFile(ROOT_TEXTURE))
		exit(NULL);
	if (!texture_player.loadFromFile(PLAYER_TEXTURE))
		exit(NULL);
	plants.push_back(Plant(&texture, 1.f, WIN_W / 2.f));
	player.init(&texture_player);
}

void	Game::handle_events(sf::Event& event) {
	switch (event.type)
	{
	case sf::Event::Closed:
		window.close();
		exit(EXIT_SUCCESS);
		break;
	case sf::Event::KeyPressed:
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			window.close();
			exit(EXIT_SUCCESS);
			break;
		}
		break;
	}
}

void	Game::update_values() {
	delta = clock.restart();

	for (Plant& plant : plants)
	{
		plant.grow(10.f, delta.asSeconds());
	}
	player.handle_movement(delta);
	player.update_position(delta);
}

void	Game::render() {
	window.clear();

	for (Plant& plant : plants)
	{
		window.draw(plant);
	}
	window.draw(player);
	window.display();
}