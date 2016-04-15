#pragma once

//Base class for entities

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class Entity {
public:
	float x, y;
	float speed;
	int health;
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void update(float dt) = 0;
	virtual const sf::RectangleShape& getRect() { return rect; };
protected:
	sf::RectangleShape rect;
};
