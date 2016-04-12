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
			int xSize, ySize;
			xSize = setting["size"]["w"];
			ySize = setting["size"]["h"];
			rect.setSize(sf::Vector2f(xSize, ySize));
			health = setting["health"];
			speed = setting["movespeed"];
			gridPosition = gridPos;
			gridSpace = gridSpc;
		} CATCH_SETTING_ERRORS(settingsFile);
	}

	void Enemy::draw(sf::RenderWindow& window) {
		window.draw(rect);
	}

	void Enemy::update(float dt) {
		switch (type) {
			case EnemyType::Goon:
				/* Move around to +- half of gridSpace on the x-axis */
				x = gridPosition * rect.getSize().x + (gridSpace / 2.f) * std::sin(Game::totalTime);
				break;
			default:  //Probably just haven't implemented this yet
				std::cerr << "Error: No defined AI for enemy " << enemyNames.at(type) << "\nDid you remember to implement it?\n";
				throw 1;
		}
		rect.setPosition(x, y);
	}
};
