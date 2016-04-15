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
		{ EnemyType::Goon, "Goon" }
	};

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
			rect.setFillColor(sf::Color::Red);
			rect.setOutlineThickness(2);
			rect.setOutlineColor(sf::Color::Blue);
			rect.setOrigin(xSize / 2.f, ySize / 2.f);
			health = setting["health"];
			speed = setting["movespeed"];
			gridPosition = gridPos;
			gridSpace = gridSpc;
		} CATCH_SETTING_ERRORS(settingsFile);
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
		switch (type) {
			case EnemyType::Goon:
				/* Move around to +- half of gridSpace on the x-axis */
				/* TODO: Something is off here, fix it */
				x = (gridSpace / 2.f) + gridPosition * gridSpace + (gridSpace / 2.f) * std::sin(Game::totalTime);
				y += Game::scrollSpeed * dt;
				break;
			default:  //Probably just haven't implemented this enemy yet
				std::cerr << "\033[1;31mError\033[0m: No defined AI for enemy " << enemyNames.at(type) << "\nDid you remember to implement it?\n";
				throw 1;
		}
		rect.setPosition(x, y);
		if (player.getRect().getGlobalBounds().intersects(rect.getGlobalBounds())) {
			player.doDamage(dt);
		}
	}

	bool Enemy::shouldDelete() {
		return y > Game::windowSize.y + (ySize / 2.f);
	}
}
