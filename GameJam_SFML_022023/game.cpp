#include "game.h"

Game::Game(const char* name, int w, int h)
{
	window.create(sf::VideoMode(w, h), name);
	window.setVerticalSyncEnabled(true);
	delta = clock.restart();
	if (!texture.loadFromFile(ROOT_TEXTURE))
		exit(NULL);
	roots.emplace_back(sf::Vector2f(WIN_W / 3, 0.f), 100.f, &texture);
	if (!texture_player.loadFromFile(PLAYER_TEXTURE))
		exit(NULL);
	player.init(&texture_player);
	if (!bg_texture.loadFromFile(BG_TEXTURE))
		exit(NULL);
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setScale(6.f, 6.f);
	bg_sprite.setPosition(0.f, -50.f);
	audio.init();
	audio.play_music();
}

void	Game::handle_events(sf::Event& event)
{
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
	case sf::Event::MouseButtonPressed:
		player.attack();
		break;
	}
}

void	Game::update_values()
{
	delta = clock.restart();

	sf::FloatRect obj;
	sf::FloatRect collider = player.get_attack_bounds();
	for (Root& root : roots)
	{
		root.update(delta.asSeconds());
		obj = root.getGlobalBounds();
		if (player.is_attacking() && collider.intersects(obj))
		{
			root.cut(collider.top);
			audio.play_sound(SOUND_HIT);
		}
	}

	player.handle_movement(delta);
	player.update_position(delta);

	if (player.is_attacking())
		audio.play_sound(SOUND_MISS);
}

void	Game::render()
{
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

	// Debug attack bounds
	if (player.is_attacking())
	{
		sf::RectangleShape shape;
		sf::FloatRect collider = player.get_attack_bounds();
		shape.setSize(sf::Vector2f(collider.width, collider.height));
		shape.setPosition(sf::Vector2f(collider.left, collider.top));
		shape.setFillColor(sf::Color(255, 0, 0, 127));
		window.draw(shape);
	}

	window.draw(player);
	window.display();
}
