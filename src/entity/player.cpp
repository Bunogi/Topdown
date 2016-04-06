#include <iostream>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>

#include "entity/player.hpp"
#include "resource.hpp"

Player::Player() {
	x = y = 0;
	speed = 40.f;
	rect = sf::RectangleShape(sf::Vector2f(80, 80));

	/*if (entTexture.loadFromFile(getResourcePath() + "/player.png")) {
		entTexture.setSmooth(false);
		entTexture.setRepeated(false);
		rect.setTexture(&entTexture);
	} */
}

void Player::draw(sf::RenderTexture& texture) {
	texture.draw(rect);
}

void Player::update(float dt) {
	float deltaPos = speed * dt;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		x -= deltaPos;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		x += deltaPos;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		y -= deltaPos;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		y += deltaPos;
	rect.setPosition(x, y);
}
