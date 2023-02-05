#include "game.h"

Explosion explooose(sf::Vector2f(200.f, 200.f), sf::Color::Green, 15);

Game::Game(const char* name, int w, int h) {
	window.create(sf::VideoMode(w, h), name);
	window.setVerticalSyncEnabled(true);
	delta = clock.restart();
	_next_growth = 0.f;
	_state = STATE_GAME;
	if (!texture.loadFromFile(ROOT_TEXTURE))
		exit(NULL);
	if (!texture_player.loadFromFile(PLAYER_TEXTURE))
		exit(NULL);
	player.init(&texture_player);
	if (!bg_texture.loadFromFile(BG_TEXTURE))
		exit(NULL);
	if (!texture_root_end.loadFromFile(ROOT_END_TEXTURE))
		exit(NULL);
	if (!enemy_texture.loadFromFile(ENEMY_TEXTURE))
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
	if (_growth_timer.getElapsedTime().asSeconds() > _next_growth)
	{
		std::mt19937 engine(std::random_device{}());
		std::uniform_real_distribution<float> play_area_x(50, WIN_W);
		std::uniform_real_distribution<float> grow_timer(1.f, 5.f);
		std::uniform_real_distribution<float> grow_size(50.f, 200.f);
		std::uniform_real_distribution<float> grow_speed(25.f, 100.f);

		_next_growth = grow_timer(engine);
		_growth_timer.restart();
		roots.emplace_back(sf::Vector2f(play_area_x(engine), 0.f), grow_size(engine), grow_speed(engine), this);
	}
}

void Game::spawner()
{
	if (_spawn_timer.getElapsedTime().asSeconds() > _next_spawn)
	{
		std::mt19937 engine(std::random_device{}());
		std::uniform_real_distribution<float> play_area_x(50, WIN_W);
		std::uniform_real_distribution<float> spawn_timer(1.f, 5.f);
		std::uniform_real_distribution<float> spawn_scale(0.75f, 2.f);
		std::uniform_real_distribution<float> spawn_speed(100.f, 200.f);

		_next_spawn = spawn_timer(engine);
		_spawn_timer.restart();
		enemies.emplace_back(sf::Vector2f(play_area_x(engine), 0.f), spawn_scale(engine), spawn_speed(engine), &enemy_texture);
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
	int i;
	for (auto root_it = roots.begin(); root_it != roots.end();)
	{
		(*root_it).update(delta.asSeconds());
		obj = (*root_it).getGlobalBounds();
		if (player.is_attacking() )
		{
			i = (*root_it).intersects(collider);

			if (i == 0)
			{
				root_it = roots.erase(root_it);
				continue;
			}
			else if(i > 0)
			{
				(*root_it).cut(i);
				audio.play_sound(SOUND_HIT);
			}
		}
		root_it++;
	}
	update_growth();
	spawner();
	explooose.Update(delta.asSeconds());
	for (Enemy& enemy : enemies)
	{
		enemy.update_position(delta, &roots);
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
	for (Enemy& enemy : enemies)
	{
		window.draw(enemy);
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