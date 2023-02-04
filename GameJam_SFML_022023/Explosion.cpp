#include "Explosion.h"

//ChatGPT
Explosion::Explosion(sf::Vector2f position, sf::Color color, int particle_count) {
	position_ = position;
	color_ = color;

	std::mt19937 engine(std::random_device{}());
	std::uniform_real_distribution<float> angle_distribution(0, 360);
	std::uniform_real_distribution<float> magnitude_distribution(0, 100);

	for (int i = 0; i < particle_count; ++i) {
		sf::RectangleShape particle(sf::Vector2f(5, 5));
		particle.setFillColor(color_);
		particle.setPosition(position_);

		float angle = angle_distribution(engine);
		float magnitude = magnitude_distribution(engine);
		sf::Vector2f velocity(std::cos(angle) * magnitude,
			std::sin(angle) * magnitude);
		velocities_.push_back(velocity);

		particles_.push_back(particle);
	}
}
void Explosion::Update(float dt) {
	for (int i = 0; i < particles_.size(); ++i) {
		sf::Vector2f velocity = velocities_[i];
		velocity.y += 9.8 * 10.f * dt;  // Apply gravity

		particles_[i].move(velocity * dt);
		velocities_[i] = velocity;
	}
}

void Explosion::Draw(sf::RenderWindow& window) {
	for (auto& particle : particles_) {
		window.draw(particle);
	}
}