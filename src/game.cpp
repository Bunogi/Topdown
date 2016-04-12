#include <iostream>
#include <vector>
#include <stack>
#include <list>

#include <libconfig.h++>

#include "game.hpp"
#include "entity/enemy.hpp"
#include "resource.hpp"
#include "configException.hpp"

namespace Game {
	//RNG for cloud positions
	std::random_device dev;
	std::mt19937 mt(dev());
	std::uniform_real_distribution<float> xDist;
	std::uniform_real_distribution<float> yDist; //Only used at the start

	std::vector<Cloud> clouds;
	sf::Texture cloudTexture;
	sf::Sprite cloudSprite;
	sf::Vector2u windowSize;

	std::list<std::vector<Enemy::Enemy>> enemies;
	std::stack<std::vector<Enemy::Enemy>> enemiesToAdd;
	std::stack<float> enemyWaveTimes;

	unsigned cloudCount = 0;
	const float cloudSpeed = 60.f;

	float totalTime = 0.f;

	float scrollSpeed = 0.0f;
	float scroll = 0.0f;

	void init(const sf::Vector2u winSize) {
		windowSize = winSize;
		xDist = std::uniform_real_distribution<float>(20.f, windowSize.x - 20.f); //Keep clouds off the edges of the screen
		yDist = std::uniform_real_distribution<float>(20.f, windowSize.x - 20.f);
		std::string filePath = getResourcePath() + "/images/cloud.png";
		if (not cloudTexture.loadFromFile(filePath)) {
			std::cerr << "Error: Failed to open file: " << filePath << "\n";
			throw 2;
		}
		else {
			cloudSprite = sf::Sprite(cloudTexture);
			cloudSprite.setColor(sf::Color(0xFF, 0xFF, 0xFF, 0x88));
		}
	}

	void loadLevel(std::string level) {
		using namespace libconfig;
		try {
			Config config;
			config.readFile(level.c_str());
			const Setting& waves = config.getRoot()["waves"];
			const Setting& info = config.getRoot()["level"];

			cloudCount = info["clouds"];
			genClouds();
			scrollSpeed = info["scrollSpeed"];

			enemies.clear();
			/* TODO: Possibly refactor */
			for (auto it = waves.end() - 1; it != waves.begin(); it--) {
				std::cerr << "Iteration: " << it - waves.begin() << "\n";
				std::vector<Enemy::Enemy> addEnems;
				float gridSpc = static_cast<float>(windowSize.x) / (*it)["wave"].getLength();
				Enemy::EnemyType type;
				int count = 0;
				for (auto &i : (*it)["wave"]) {
					int enemy = (*it)["wave"][count];
					addEnems.push_back(Enemy::Enemy(static_cast<Enemy::EnemyType>(enemy), count, gridSpc));
					count++;
				}
				enemyWaveTimes.push((*it)["pos"]);
				enemiesToAdd.push(addEnems);
			}
		} CATCH_SETTING_ERRORS(level);
	}

	void update(float dt) {
		totalTime += dt;
		
	}

	void genClouds() {
		clouds.clear();
		for (unsigned i = 0; i < cloudCount; i++) {
			clouds.push_back({xDist(mt), yDist(mt)});
		}
	}

	void drawClouds(sf::RenderWindow& window, float dt) {
		for (auto &i : clouds) {
			i.y += cloudSpeed * dt;
			if (i.y > windowSize.y) {
				i.y = 0;
				i.x = xDist(mt);
			}
			cloudSprite.setPosition(i.x, i.y);
			window.draw(cloudSprite);
		}
	}
};
