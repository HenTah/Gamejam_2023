#include "game.h"

Game::Game(const char *name, int w, int h) {
	window.create(sf::VideoMode(w, h), name);
	window.setVerticalSyncEnabled(true);
	delta = clock.restart();
}

void	Game::handle_events(sf::Event& event) {
	if (event.type == sf::Event::Closed)
		window.close();
}

void	Game::update_values() {
	delta = clock.restart();
	
	/*for (Plant& plant : plants)
	{
		plant.update_leaves(delta);
	}*/
}

void	Game::render() {
	window.clear();

	/*for (Plant& plant : plants)
	{
		window.draw(plant);
	}*/

	window.display();
}