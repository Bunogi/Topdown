#include <libconfig.h++>
#include <iostream>

#include "entity/enemy.hpp"
#include "resource.hpp"

namespace Enemy {
	using namespace libconfig;
	const std::map<EnemyType, const char *> enemyNames {
		{ EnemyType::NoShoot, "NoShoot" }
	};

	Enemy::Enemy(EnemyType setType) : type(setType) {
		std::string settingsFile = getResourcePath() + "entities/enemies/" + enemyNames.at(type);
		rect = sf::RectangleShape();
		try {
			Config config;
			config.readFile(settingsFile.c_str());
			const Setting& setting = config.getRoot()["entity"];
			int xSize, ySize;
			xSize = setting["size"]["x"];
			ySize = setting["size"]["y"];
			rect.setSize(sf::Vector2f(xSize, ySize));
			health = setting["health"];
			speed = setting["movespeed"];
		} catch (SettingNotFoundException &ex) {
			std::cerr << "Error: Failed to find setting : " << ex.getPath() << " in file " << settingsFile << ".\n";
			throw 1;
		} catch (ParseException &ex) {
			std::cerr << "Error: Syntax error : " << ex.getFile() << ": " << ex.getLine() << " : " << ex.getError() << "\n";
			throw 1;
		}
	}

	void Enemy::draw(sf::RenderWindow& window) {
		window.draw(rect);
	}

	void Enemy::update(float dt) {
		/* TODO: Run AI and whatnot */
	}
};
