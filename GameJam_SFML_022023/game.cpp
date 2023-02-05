#include "main.h"
//Explosion explooose(sf::Vector2f(200.f, 200.f), sf::Color::Green, 15);


Game::Game() {
	sf::Vector2f	world = sf::Vector2f((float)WORLD_W, (float)WORLD_H);

	window.create(sf::VideoMode(WIN_W, WIN_H), GAME_NAME);
	window.setVerticalSyncEnabled(true);
	view.setCenter(world / 2.f);
	view.setSize(sf::Vector2f((float)WIN_W, (float)WIN_H));
	window.setView(view);

	score = sf::Time();
	delta = clock.restart();
	_next_growth = 0.f;
	_state = STATE_GAME;
	if (!root_texture.loadFromFile(ROOT_TEXTURE))
		exit(NULL);
	if (!player_texture.loadFromFile(PLAYER_TEXTURE))
		exit(NULL);
	player.init(&player_texture);
	if (!bg_texture.loadFromFile(BG_TEXTURE))
		exit(NULL);
	if (!texture_root_end.loadFromFile(ROOT_END_TEXTURE))
		exit(NULL);
	if (!enemy_texture.loadFromFile(ENEMY_TEXTURE))
		exit(NULL);
	if (!particle_texture.loadFromFile(PARTICLE_TEXTURE))
		exit(NULL);
	if (!ui_texture.loadFromFile(UI_TEXTURE))
		exit(NULL);
	if (!font.loadFromFile(FONT_FILE))
		exit(NULL);
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setScale(BG_SCALE, BG_SCALE);
	bg_sprite.setPosition(0.f, -50.f);
	audio.init();
	menu.init(this);
	audio.play_music();
	_init_ui();
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
		std::uniform_real_distribution<float> play_area_x(50, WORLD_W);
		std::uniform_real_distribution<float> grow_timer(1.f, 5.f);
		std::uniform_real_distribution<float> grow_size(100.f, 200.f);
		std::uniform_real_distribution<float> grow_speed(25.f, 100.f);

		_next_growth = grow_timer(engine);
		_growth_timer.restart();
		roots.emplace_back(sf::Vector2f(play_area_x(engine), 0.f), grow_size(engine), grow_speed(engine), this);
		particles.emplace_back(sf::Vector2f(roots.back().getGlobalBounds().left, 0.f), sf::Color::Cyan, 10, particle_texture);
	}
}

void Game::spawner()
{
	if (_spawn_timer.getElapsedTime().asSeconds() > _next_spawn)
	{
		std::mt19937 engine(std::random_device{}());
		std::uniform_real_distribution<float> play_area_x(50, WORLD_W);
		std::uniform_real_distribution<float> spawn_timer(1.f, 5.f);
		std::uniform_real_distribution<float> spawn_scale(0.75f, 1.25f);
		std::uniform_real_distribution<float> spawn_speed(50.f, 200.f);

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
	score += delta;

	sf::FloatRect obj;
	sf::FloatRect collider = player.get_attack_bounds();
	for (auto root_it = roots.begin(); root_it != roots.end();)
	{
		(*root_it).update(delta.asSeconds());
		obj = (*root_it).getGlobalBounds();
		if (player.is_attacking() )
		{
			int i = (*root_it).intersects(collider);

			if (i == 0)
			{
				root_it = roots.erase(root_it);
				continue;
			}
			else if(i > 0)
			{
				(*root_it).cut(i);
				obj = (*root_it).getGlobalBounds();
				particles.emplace_back(sf::Vector2f(obj.left + obj.width / 2, collider.top), sf::Color::Red, 25, particle_texture);
				audio.play_sound(SOUND_HIT);
			}
		}
		root_it++;
	}
	update_growth();
	spawner();
	for (auto particle_it = particles.begin(); particle_it != particles.end();)
	{
		(*particle_it).Update(delta.asSeconds());
		if ((*particle_it).get_alive() > 2.f)
		{
			particle_it = particles.erase(particle_it);
		}
		else
			particle_it++;
	}

	player.handle_movement(delta);

	_grounded_enemies = 0;
	for (auto enemy_it = enemies.begin(); enemy_it != enemies.end();)
	{
		if ((*enemy_it).is_grounded())
			_grounded_enemies++;
		if ((*enemy_it).get_health() == 100.f)
		{
			if (player.check_slime_bounce(*enemy_it))
				(*enemy_it).hit();
			
		}
		(*enemy_it).update_position(delta, &roots);
		if ((*enemy_it).get_health() < 0)
		{
			enemy_it = enemies.erase(enemy_it);
		}
		else
			enemy_it++;
	}

	player.update_position(delta);
	_update_ui();

	if (player.is_attacking())
		audio.play_sound(SOUND_MISS);
}

void	Game::render()
{
	handle_view();
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
	for (Explosion& particle : particles)
	{
		particle.Draw(window);
	}
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
	_draw_ui();

	if (_state == STATE_MENU)
		menu.render(window);
	window.display();
}

void	Game::handle_view()
{
	float	player_center = player.getPosition().x + PLAYER_W / 2.f;

	if (player_center < WIN_W / 2.f || player_center > (float)WORLD_W - WIN_W / 2.f)
		return;

	float	offset = player_center - view.getCenter().x;

	view.move(sf::Vector2f(fabsf(offset) * offset / (WIN_W / 2.f) * 0.5f, 0.f));
	window.setView(view);

	float	bg_offset = -(window.getView().getCenter().x - WIN_W / 2.f);
	bg_sprite.setOrigin(sf::Vector2f(bg_offset / BG_RELATIVE_SCALE, 0.f));
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

void	Game::_init_ui()
{
	_overwhelming_bar_frame.setTexture(&ui_texture);
	_overwhelming_bar_frame.setSize(sf::Vector2f(395.f, 37.f));
	_overwhelming_bar_frame.setTextureRect(sf::IntRect(569, 98, 395, 37));
	_overwhelming_bar_frame.setScale(1.f, 1.f);
	_overwhelming_bar_frame.setPosition(sf::Vector2f(10.f, 10.f));

	_overwhelming_bar.setTexture(&ui_texture);
	_overwhelming_bar.setSize(sf::Vector2f(1.f, 37.f));
	_overwhelming_bar.setTextureRect(sf::IntRect(569, 34, 1, 37));
	_overwhelming_bar.setScale(1.f, 1.f);
	_overwhelming_bar.setPosition(sf::Vector2f(10.f, 10.f));

	_text.setFont(font);
	_text.setString("Score");
	_text.setCharacterSize(32);
	_text.setFillColor(sf::Color::White);
	_text.setPosition(sf::Vector2f(10.f, 47.f));
}

void	Game::_update_ui()
{
	_overwhelming_bar.setSize(sf::Vector2f(_grounded_enemies * 395 / OVERWHELM_ENEMY_COUNT, 37.f));
	_overwhelming_bar.setTextureRect(sf::IntRect(569, 34, _grounded_enemies * 395 / OVERWHELM_ENEMY_COUNT, 37));
}

void	Game::_draw_ui()
{
	float	offset = -(window.getView().getCenter().x - WIN_W / 2.f);

	_overwhelming_bar_frame.setOrigin(sf::Vector2f(offset, 0.f));
	_overwhelming_bar.setOrigin(sf::Vector2f(offset, 0.f));
	_text.setOrigin(sf::Vector2f(offset, 0.f));
	window.draw(_overwhelming_bar_frame);
	window.draw(_overwhelming_bar);
	window.draw(_text);
}