#include "main.h"

Game::Game()
{
	sf::Vector2f	world = sf::Vector2f((float)WORLD_W, (float)WORLD_H);

	window.create(sf::VideoMode(WIN_W, WIN_H), GAME_NAME);
	window.setVerticalSyncEnabled(true);
	view.setCenter(world / 2.f);
	view.setSize(sf::Vector2f((float)WIN_W, (float)WIN_H));
	window.setView(view);
	_enemies_left = false;
	_enemies_right = false;
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
		double difficulty = score.asSeconds();
		difficulty * 0.001;
		if (difficulty > 0.49)
			difficulty = 0.49;
		double growth_cap = score.asSeconds();
		if (growth_cap > 500)
			growth_cap = 500;
		std::mt19937 engine(std::random_device{}());
		std::uniform_real_distribution<float> play_area_x(50, WORLD_W);
		std::uniform_real_distribution<float> grow_timer(1.5f - difficulty, 3.f - growth_cap * (2 / 500) - difficulty);
		std::uniform_real_distribution<float> grow_size(100.f, 200.f);
		std::uniform_real_distribution<float> grow_speed(50.f, 200.f);

		_next_growth = grow_timer(engine);
		_growth_timer.restart();
		roots.emplace_back(sf::Vector2f(play_area_x(engine), 0.f), grow_size(engine), grow_speed(engine), this);
		particles.emplace_back(sf::Vector2f(roots.back().getGlobalBounds().left, 0.f), sf::Color::Cyan, 10, particle_texture);
	}
}

void Game::spawner()
{
	static int direction;
	double difficulty = score.asSeconds();
	difficulty * 0.0001;
	if (difficulty > 0.19)
		difficulty = 0.19;
	double spawn_cap = score.asSeconds();
	if (spawn_cap > 900)
		spawn_cap = 900;
	if (!direction)
		direction = 1;
	if (_spawn_timer.getElapsedTime().asSeconds() > _next_spawn)
	{
		std::mt19937 engine(std::random_device{}());
		std::uniform_real_distribution<float> play_area_x(50, WORLD_W);
		std::uniform_real_distribution<float> spawn_timer(0.2f - difficulty, 2.f - spawn_cap * (1.5 / 900) - difficulty);
		std::uniform_real_distribution<float> spawn_scale(0.75f, 1.25f);
		std::uniform_real_distribution<float> spawn_speed(200.f, 400.f);

		_next_spawn = spawn_timer(engine);
		_spawn_timer.restart();

		enemies.emplace_back(sf::Vector2f(play_area_x(engine), 0.f), spawn_scale(engine), direction * spawn_speed(engine), &enemy_texture);
		direction = -direction;
	}
}

void	Game::update_values()
{
	_enemies_left = _enemies_right = 0;
	if (_state == STATE_GAMEOVER)
		return;

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
		if (player.is_attacking())
		{
			int i = (*root_it).intersects(collider);
			if (i >= 0)
			{
				particles.emplace_back(sf::Vector2f(obj.left + obj.width / 2, collider.top), sf::Color::Red, 25, particle_texture);
				audio.play_sound(SOUND_HIT);
				for (auto& enemy : enemies)
				{
					if (obj.contains(enemy.getPosition()))
					{
						if (i == 0)
							enemy.hit();
						enemy.set_root();
					}
				}
				if (i == 0)
				{
					root_it = roots.erase(root_it);
					continue;
				}
				(*root_it).cut(i);
			}
		}
		if ((*root_it).getState() && (*root_it).get_pos() < (window.getView().getCenter().x - WIN_W / 2.f))
			_enemies_left = true;
		if ((*root_it).getState() && (*root_it).get_pos() > (window.getView().getCenter().x + WIN_W / 2.f))
			_enemies_right = true;
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

	if (_grounded_enemies > OVERWHELM_ENEMY_COUNT)
		_state = STATE_GAMEOVER;

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
	for (Explosion& particle : particles)
	{
		particle.Draw(window);
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
	float	view_center = view.getCenter().x;

	if ((view_center <= WIN_W / 2.f && player_center <= WIN_W / 2.f)
		|| (view_center >= (float)WORLD_W - WIN_W / 2.f && player_center >= (float)WORLD_W - WIN_W / 2.f))
		return;

	float	offset = player_center - view_center;

	view.move(sf::Vector2f(fabsf(offset) * offset / (WIN_W / 2.f) * 0.5f, 0.f));
	window.setView(view);

	float	bg_offset = -(window.getView().getCenter().x - WIN_W / 2.f) + 20.f;
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

	_enemies_left_icon.setTexture(&ui_texture);
	_enemies_left_icon.setFillColor(sf::Color::Red);
	_enemies_left_icon.setSize(sf::Vector2f(118.f, 118.f));
	_enemies_left_icon.setTextureRect(sf::IntRect(144, 0, 118, 118));
	_enemies_left_icon.setScale(1.f, 1.f);
	_enemies_left_icon.setOrigin(sf::Vector2f(59.f, 59.f));
	_enemies_left_icon.setRotation(180.f);
	_enemies_left_icon.setPosition(sf::Vector2f(138.f, WIN_H / 2.f));

	_enemies_right_icon.setTexture(&ui_texture);
	_enemies_right_icon.setFillColor(sf::Color::Red);
	_enemies_right_icon.setSize(sf::Vector2f(118.f, 118.f));
	_enemies_right_icon.setTextureRect(sf::IntRect(144, 0, 118, 118));
	_enemies_right_icon.setScale(1.f, 1.f);
	_enemies_right_icon.setPosition(sf::Vector2f(WIN_W - 138.f, WIN_H / 2.f - 59.f));

	_text.setFont(font);
	_text.setString("Score");
	_text.setCharacterSize(32);
	_text.setFillColor(sf::Color::White);
	_text.setPosition(sf::Vector2f(10.f, 47.f));

	_text_score.setFont(font);
	_text_score.setString("0");
	_text_score.setCharacterSize(32);
	_text_score.setFillColor(sf::Color::White);
	_text_score.setPosition(sf::Vector2f(120.f, 47.f));

	_text_gameover.setFont(font);
	_text_gameover.setString("GAME OVER");
	_text_gameover.setCharacterSize(32);
	_text_gameover.setFillColor(sf::Color::White);
	_text_gameover.setPosition(sf::Vector2f((float)((WIN_W - 100) / 2), (float)(WIN_H / 2)));

	_bg_shadow.setSize(sf::Vector2f((float)(WIN_W), (float)(WIN_H)));
	_bg_shadow.setFillColor(sf::Color(0, 0, 0, 200));
}

void	Game::_update_ui()
{
	_overwhelming_bar.setSize(sf::Vector2f(_grounded_enemies * 395 / OVERWHELM_ENEMY_COUNT, 37.f));
	_overwhelming_bar.setTextureRect(sf::IntRect(569, 34, (int)(_grounded_enemies * 395 / OVERWHELM_ENEMY_COUNT), 37));
	_text_score.setString(std::to_string((int)pow(1.06f, 1.f + score.asSeconds()) + (int)score.asSeconds()));
}

void	Game::_draw_ui()
{
	float	offset = -(window.getView().getCenter().x - WIN_W / 2.f);

	if (_state == STATE_GAMEOVER)
	{
		_bg_shadow.setOrigin(sf::Vector2f(offset, 0.f));
		_text_gameover.setOrigin(sf::Vector2f(offset, 0.f));
		window.draw(_bg_shadow);
		window.draw(_text_gameover);
	}
	_overwhelming_bar_frame.setOrigin(sf::Vector2f(offset, 0.f));
	_overwhelming_bar.setOrigin(sf::Vector2f(offset, 0.f));
	_text.setOrigin(sf::Vector2f(offset, 0.f));
	_text_score.setOrigin(sf::Vector2f(offset, 0.f));
	_enemies_left_icon.setPosition(sf::Vector2f(
		-offset + 79.f,
		_enemies_left_icon.getPosition().y));
	_enemies_right_icon.setOrigin(sf::Vector2f(offset, 0.f));
	window.draw(_overwhelming_bar_frame);
	window.draw(_overwhelming_bar);
	window.draw(_text);
	window.draw(_text_score);

	if (_enemies_left)
		window.draw(_enemies_left_icon);

	if (_enemies_right)
		window.draw(_enemies_right_icon);
}
