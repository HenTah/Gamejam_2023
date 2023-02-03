#include "game.h"

Game::Game(const char* name, int w, int h) {
	window.create(sf::VideoMode(w, h), name);
	window.setVerticalSyncEnabled(true);
	delta = clock.restart();
	if (!texture.loadFromFile(ROOT_TEXTURE))
		exit(NULL);
	roots.emplace_back(sf::Vector2f(WIN_W / 3, 0.f), 100.f, &texture);
	if (!texture_player.loadFromFile(PLAYER_TEXTURE))
		exit(NULL);
	player.init(&texture_player);
	if (!bg_texture.loadFromFile("./assets/bg9.png"))
		exit(NULL);
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setScale(6.f, 6.f);
	bg_sprite.setPosition(0.f, -50.f);
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
	sf::FloatRect obj;
	sf::FloatRect collider = player.getGlobalBounds();
	for (Root& root : roots)
	{
		root.update(delta.asSeconds());
		obj = root.getGlobalBounds();
		if (collider.intersects(obj))
		{
			root.cut(collider.top);
		}
	}

	player.handle_movement(delta);
	player.update_position(delta);
}

void	Game::render() {
	window.clear();
	window.draw(bg_sprite);
	sf::RectangleShape shape;
	for (Root& root : roots)
	{
		root.draw(window);
		sf::FloatRect bounds = root.getGlobalBounds();
		shape.setSize(sf::Vector2f(bounds.width, bounds.height));
		shape.setPosition(sf::Vector2f(bounds.left, bounds.top));
	}
	//window.draw(shape); //collider debug view
	window.draw(player);
	window.display();
}