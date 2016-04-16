#include <iostream>
#include <cmath>

#include <libconfig.h++>

#include "entity/enemy.hpp"
#include "resource.hpp"
#include "configException.hpp"
#include "game.hpp"

namespace Enemy {
	using namespace libconfig;
	const std::map<EnemyType, std::string> enemyNames {
		{ EnemyType::Goon, "Goon" },
		{ EnemyType::Diver, "Diver" }
	};

	std::map<EnemyType, sf::Texture> enemyTextures;

	void load() {
		for (auto it = enemyNames.begin(); it != enemyNames.end(); it++) {
			sf::Texture& tex = enemyTextures[it->first];
			std::string textureFile = getResourcePath() + "/images/" + enemyNames.at(it->first) + ".png";
			if (not tex.loadFromFile(textureFile)) {
				std::cerr << "\033[1;31mError\033[0m: Failed to load texture: " << textureFile << "\n";
				throw 1;
			}
		}
	}

	/* TODO: Not require reading the settings file every time we create a new enemy */
	Enemy::Enemy(EnemyType setType, unsigned gridPos, float gridSpc) : type(setType) {
		std::string settingsFile = getResourcePath() + "/entities/enemies/" + enemyNames.at(type) + ".cfg";
		rect = sf::RectangleShape();
		try {
			Config config;
			config.readFile(settingsFile.c_str());
			const Setting& setting = config.getRoot()["entity"];
			xSize = setting["size"]["w"];
			ySize = setting["size"]["h"];
			rect.setSize(sf::Vector2f(xSize, ySize));
			rect.setOrigin(xSize / 2.f, ySize / 2.f);
			rect.setTexture(&enemyTextures[type]);
			health = setting["health"];
			speed = setting["movespeed"];
			gridPosition = gridPos;
			gridSpace = gridSpc;
			x = (gridSpace / 2.f) + gridPosition * gridSpace;
			y = 0;
		} CATCH_SETTING_ERRORS(settingsFile);
		xOffset = yOffset = 0.f;
	}

	void Enemy::draw(sf::RenderWindow& window) {
		window.draw(rect);
	}

	/* Well, this is one way to "delete" virtual functions */
	void Enemy::update(float) {
		std::cerr << "\033[1;31mError\033[0m: Enemy::update(float) should never be called!\n";
		throw 1;
	}

	void Enemy::update(float dt, Player& player) {
		y += Game::scrollSpeed * dt;
		switch (type) {
			case EnemyType::Goon:
				/* Move around to +- half of gridSpace on the x-axis */
				/* TODO: Something is off here, fix it */
				xOffset = (gridSpace / 2.f) * std::sin(Game::totalTime);
				break;
			case EnemyType::Diver:
				yOffset = (gridSpace / 2.f) * std::abs(std::sin(Game::totalTime));
				break;
			default:  //Probably just haven't implemented this enemy yet
				std::cerr << "\033[1;31mError\033[0m: No defined AI for enemy: " << enemyNames.at(type) << ".\nDid you remember to implement it?\n";
				throw 1;
		}
		rect.setPosition(x + xOffset, y + yOffset);
		if (player.getRect().getGlobalBounds().intersects(rect.getGlobalBounds())) {
			player.doDamage(dt);
		}
	}

	bool Enemy::shouldDelete() {
		return y > Game::windowSize.y + (ySize / 2.f) or health <= 0;
	}

	void Enemy::doDamage() {
		health--;
	}
}
