#include "main.h"

//ChatGPT
Explosion::Explosion(sf::Vector2f position, sf::Color color, int particle_count, sf::Texture &texture) {
	position_ = position;
	color_ = color;
	_alive.restart();
	std::mt19937 engine(std::random_device{}());
	std::uniform_real_distribution<float> angle_distribution(0, 360);
	std::uniform_real_distribution<float> magnitude_distribution(0, 100);

	for (int i = 0; i < particle_count; ++i) {
		sf::Sprite particle(texture);
		particle.setColor(color_);
		particle.setScale(sf::Vector2f(5.f,5.f));
		particle.setPosition(position_);

		float angle = angle_distribution(engine);
		float magnitude = magnitude_distribution(engine);
		sf::Vector2f velocity(std::cos(angle) * magnitude,
			std::sin(angle) * magnitude);
		velocities_.push_back(velocity);

		particles_.push_back(particle);
	}
}
double Explosion::get_alive()
{
	return _alive.getElapsedTime().asSeconds();
}
void Explosion::Update(float dt) {
	for (size_t i = 0; i < particles_.size(); ++i) {
		sf::Vector2f velocity = velocities_[i];
		velocity.y += 9.8 * 10.f * dt;  // Apply gravity

		particles_[i].scale(sf::Vector2f(1.f - 5.f * dt * (1 - 0.9f), 1.f - 5.f * dt * (1 - 0.9f)));
		particles_[i].move(velocity * dt);
		velocities_[i] = velocity;
	}
}

void Explosion::Draw(sf::RenderWindow& window) {
	for (auto& particle : particles_) {
		window.draw(particle);
	}
}