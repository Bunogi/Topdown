#include <iostream>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>

#include <libconfig.h++>

#include "entity/player.hpp"
#include "resource.hpp"
#include "configException.hpp"
#include "game.hpp"

Player::Player() {
	using namespace libconfig;
	std::string settingsFile = getResourcePath() + "/entities/player.cfg";
	try {
		Config config;
		config.readFile(settingsFile.c_str());
		const Setting& setting = config.getRoot()["entity"];
		int xSize, ySize;
		xSize = setting["size"]["w"];
		ySize = setting["size"]["h"];
		x = Game::windowSize.x / 2.f + xSize / 2.f;
		y = Game::windowSize.y / 2.f + Game::windowSize.y / 3.f;
		rect.setSize(sf::Vector2f(xSize, ySize));
		health = setting["health"];
		speed = setting["movespeed"];
		rect.setFillColor(sf::Color::Green);
	} CATCH_SETTING_ERRORS;

	/*if (entTexture.loadFromFile(getResourcePath() + "/player.png")) {
		entTexture.setSmooth(false);
		entTexture.setRepeated(false);
		rect.setTexture(&entTexture);
	} */
}

void Player::draw(sf::RenderWindow& window) {
	window.draw(rect);
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
