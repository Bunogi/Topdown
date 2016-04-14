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
		flickerSpeed = setting["flickerSpeed"];
		damageInvulnResetTime = setting["invulnTime"];
		rect.setFillColor(sf::Color::White);
	} CATCH_SETTING_ERRORS(settingsFile);

	if (entTexture.loadFromFile(getResourcePath() + "/images/player.png")) {
		entTexture.setSmooth(false);
		entTexture.setRepeated(false);
		rect.setTexture(&entTexture);
	}
}

void Player::draw(sf::RenderWindow& window) {
	if (damageInvulnTime > 0.f) {
		if (flickerTime >= 0.f)
			rect.setFillColor(sf::Color(0xFF, 0xFF, 0xFF, 0x88));
		else {
			rect.setFillColor(sf::Color::White);
			flickerTime = flickerSpeed;
		}
	}
	else
		rect.setFillColor(sf::Color::White);
	window.draw(rect);
	std::cerr << "\rHealth: " << health;
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

	if (damageInvulnTime > 0.f) {
		damageInvulnTime -= dt;
		flickerTime -= dt;
	}
}

const sf::RectangleShape& Player::getRect() {
	return rect;
}

void Player::doDamage(float dt) {
	if (damageInvulnTime <= 0.f) {
		damageInvulnTime = damageInvulnResetTime;
		health--;
	}
}
