#include "main.h"
#include <iostream>

	//Plant::Plant()
	//{
	//	this->setFillColor(sf::Color::Red);
	//	this->setSize(sf::Vector2f(100.f, 100.f * 2));
	//	this->setOrigin(this->getSize().x / 2, 0.f);
	//	this->setPosition(sf::Vector2f(100.f, 0.f));

	//}
	Plant::Plant(sf::Texture* tex, double size, double pos)
	{
		if (!tex)
			exit(NULL);
		this->setTexture(tex);
		tex->setRepeated(true);
		this->setSize(sf::Vector2f(tex->getSize().x,tex->getSize().y));
		this->setTextureRect(sf::IntRect(0, 0, this->getSize().x, this->getSize().y));
		this->setOrigin(this->getSize().x / 2, 0.f);
		this->setScale(size, size);
		this->setPosition(sf::Vector2f(pos,0.f));
	}

	void Plant::grow(double speed, double time)
	{
		sf::Vector2f size = this->getSize();
		std::cout << size.y << std::endl;
		this->setTextureRect(sf::IntRect(0, 0, 100, size.y + speed * time));

		this->setSize(sf::Vector2f(size.x, size.y + speed * time));

	}
