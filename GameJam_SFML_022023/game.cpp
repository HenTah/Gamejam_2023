#include "game.h"



Explosion explooose(sf::Vector2f(200.f, 200.f), sf::Color::Green, 15);

Game::Game(const char* name, int w, int h) {
	window.create(sf::VideoMode(w, h), name);
	window.setVerticalSyncEnabled(true);
	delta = clock.restart();
	_growth_value = 0.f;
	_state = STATE_GAME;
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
	menu.init();
	audio.play_music();
}

void	Game::handle_events(sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::Closed:
		this->exit_game();
		break;
	case sf::Event::KeyPressed:
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			if (_state != STATE_MENU)
				_state = STATE_MENU;
			else
				_state = STATE_GAME;
			break;
		}
		break;
	case sf::Event::MouseButtonPressed:
		player.attack();
		break;
	}
}

void	Game::update_growth()
{
	if (growth_timer.getElapsedTime().asSeconds() > _growth_value)
	{
		std::mt19937 engine(std::random_device{}());
		std::uniform_real_distribution<float> play_area_x(50, WIN_W);
		std::uniform_real_distribution<float> grow_timer(1.f, 5.f);
		std::uniform_real_distribution<float> grow_size(50.f, 200.f);

		_growth_value = grow_timer(engine);
		growth_timer.restart();
		roots.emplace_back(sf::Vector2f(play_area_x(engine), 0.f), grow_size(engine), &texture);
	}
}

void	Game::update_values()
{
	delta = clock.restart();

	if (_state == STATE_MENU)
	{
		menu.update_values(this);
		menu.handle_actions(this);
		return;
	}

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
	update_growth();
	explooose.Update(delta.asSeconds());
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
	explooose.Draw(window);

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

	if (_state == STATE_MENU)
		menu.render(window);
	window.display();
}

void	Game::set_state(int state)
{
	_state = state;
}

void	Game::exit_game()
{
	window.close();
	exit(EXIT_SUCCESS);
}