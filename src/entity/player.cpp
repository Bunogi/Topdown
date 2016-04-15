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
		heartDistance = setting["heartSpace"];
	} CATCH_SETTING_ERRORS(settingsFile);

	if (entTexture.loadFromFile(getResourcePath() + "/images/player.png")) {
		entTexture.setSmooth(false);
		entTexture.setRepeated(false);
		rect.setTexture(&entTexture);
	}

	if (heartTexture.loadFromFile(getResourcePath() + "/images/heart.png")) {
		heartTexture.setSmooth(false);
		heartTexture.setRepeated(false);
		heartRect = sf::RectangleShape(sf::Vector2f(36.f, 36.f));
		heartRect.setTexture(&heartTexture);
		heartWidth = heartRect.getSize().x;
		heartYPos = Game::windowSize.y - heartWidth; //The heart texture is always square
		heartRect.setPosition(0.f, heartYPos);
	}

	isFlickering = false;
}

void Player::draw(sf::RenderWindow& window) {
	if (damageInvulnTime > 0.f) {
		if (isFlickering)
			rect.setFillColor(sf::Color(0xFF, 0xFF, 0xFF, 0x88));
		else {
			rect.setFillColor(sf::Color::White);
		}
	}
	else
		rect.setFillColor(sf::Color::White);
	window.draw(rect);
	for (int i = 0; i < health; i++) {
		heartRect.setPosition(i * heartWidth + heartDistance, heartYPos);
		window.draw(heartRect);
	}
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
		if (flickerTime <= 0.f) {
			isFlickering = not isFlickering;
			flickerTime = flickerSpeed;
		}
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
