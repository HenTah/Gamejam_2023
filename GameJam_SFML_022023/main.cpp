#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "main.h"
#include <chrono>
#include <cmath>
#include <random>
#include <iostream>

constexpr int window_width = 800;
constexpr int window_height = 600;
constexpr int cloud_lines = 100;
constexpr float cloud_max_size = 200.f;
constexpr int cloud_max_alpha = 128;
constexpr int dissolve_duration = 2000; // in milliseconds
constexpr float line_speed = 200.f;

int main()
{
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "SFML works!");
    window.setFramerateLimit(60);

    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<float> angle_dist(0.f, 2 * M_PI);
    std::uniform_real_distribution<float> size_dist(0.f, cloud_max_size);
    std::uniform_int_distribution<int> alpha_dist(0, cloud_max_alpha);

    sf::VertexArray lines(sf::Lines, cloud_lines * 2);
    for (int i = 0; i < cloud_lines; ++i) {
        float angle = angle_dist(rng);
        float size = size_dist(rng);
        std::cout << size << "\n";

        int alpha = alpha_dist(rng);
        sf::Color color(128, 255, 0, alpha);
        lines[i * 2].color = color;
        lines[i * 2 + 1].color = color;
        lines[i * 2].position = sf::Vector2f(window_width / 2, window_height / 2);
        lines[i * 2 + 1].position = sf::Vector2f(
            window_width / 2 + std::cos(angle) * size,
            window_height / 2 + std::sin(angle) * size);
    }

    sf::CircleShape shape(100.f);
    sf::VertexArray pixels(sf::Points, 1);
    pixels[0].position = sf::Vector2f(100.f, 100.f);
    pixels[0].color = sf::Color::Red;
    sf::Texture texture;
    if (!texture.loadFromFile("C:\\Downloads\\texture4.png"))
        return 1;
    sf::Music music;
    if (!music.openFromFile("\\\\NUKOWAI\\Music_All\\MUSIC\\SingleTracks\\EncoderLoop.flac"))
        return 1;
    music.setLoop(true);
    music.play();
    std::vector<sf::Vector2f> vertices = {
    sf::Vector2f(100, 100),
    sf::Vector2f(200, 200),
    sf::Vector2f(300, 100)
    };

    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setTexture(&texture);
    triangle.setTextureRect(sf::IntRect(100, 100, 100, 100));

    for (int i = 0; i < 3; i++)
        triangle.setPoint(i, vertices[i]);

    float angle = 0.0f;

    sf::VertexBuffer buffer(sf::Points);
    buffer.create(1);

    sf::Vertex vertex;
    vertex.position = sf::Vector2f(50.f, 50.f);
    vertex.color = sf::Color::Magenta;
    buffer.update(&vertex, 1, 0);

    shape.setFillColor(sf::Color::Green);

    sf::Vector2f center = (vertices[0] + vertices[1] + vertices[2]) / 3.0f;
    triangle.setPosition(center);

    sf::Clock clock;
    sf::Time delta;
    int elapsed = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        elapsed = clock.getElapsedTime().asMilliseconds();
        /*if (elapsed >= dissolve_duration) {
            break;
        }*/
        std::cout << elapsed << "\n";

        for (int i = 0; i < cloud_lines; ++i) {
            float alpha = alpha_dist(rng) * elapsed / dissolve_duration;
 /*           lines[i * 2].color.a = alpha;
            lines[i * 2 + 1].color.a = alpha;*/
            lines[i * 2].position = sf::Vector2f(window_width / 2, window_height / 2);
            lines[i * 2 + 1].position += sf::Vector2f(std::cos(angle_dist(rng)) * line_speed * elapsed,
                std::sin(angle_dist(rng)) * line_speed * elapsed);
        }
        
        delta = clock.restart();
        angle += 0.01f * 1000.f * delta.asSeconds();
        if (angle > 360.0f)
            angle -= 360.0f;

        triangle.setPosition(sf::Vector2f(200.f, 200));
        triangle.setOrigin((vertices[0] + vertices[1] + vertices[2]) / 3.0f);

        triangle.setRotation(angle);

        window.clear();
        //window.draw(shape);
        sf::Vertex pixs;
        pixs.position.x = 25.f;
        pixs.position.y = 25.f;
        window.draw(&pixs, 1, sf::Points);
        window.draw(pixels);
        window.draw(buffer);
        window.draw(lines);
        window.draw(triangle);

        window.display();
    }

    return 0;
}