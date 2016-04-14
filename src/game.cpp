#include <iostream>
#include <vector>
#include <stack>
#include <list>

#include <libconfig.h++>

#include "game.hpp"
#include "entity/enemy.hpp"
#include "entity/player.hpp"
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

	//TODO: Store pointers to these vectors instead, to run faster
	std::list<std::vector<Enemy::Enemy>> enemies;
	std::stack<std::vector<Enemy::Enemy>> enemiesToAdd;
	std::stack<float> enemyWaveTimes;

	unsigned cloudCount = 0;
	const float cloudSpeed = 60.f;

	float totalTime = 0.f;

	float scrollSpeed = 0.f;
	float scroll = 0.f;

	float cloudTexH = 0.f;

	Player *player = nullptr;

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
			cloudTexH = cloudTexture.getSize().x;
		}
	}

	void loadLevel(std::string level) {
		if (player != nullptr) {
			delete player;
		}
		player = new Player;

		using namespace libconfig;
		try {
			Config config;
			config.readFile(level.c_str());
			const Setting& waves = config.getRoot()["waves"];
			const Setting& info = config.getRoot()["level"];

			cloudCount = info["clouds"];
			genClouds();
			scrollSpeed = info["scrollSpeed"];
			scroll = 0.0f;

			enemies.clear();
			/* TODO: Possibly refactor */
			for (auto it = waves.end() - 1; it != waves.begin() - 1; it--) {
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


	void update(float dt, sf::RenderWindow& window) {
		totalTime += dt;
		scroll += scrollSpeed * dt;

		//Insert new enemies
		if (enemyWaveTimes.size() > 0) {
			if (enemyWaveTimes.top() <= scroll) { //Probably won't ever be equal to scroll. It's just too unlikely
				enemies.emplace_back(enemiesToAdd.top());
				enemiesToAdd.pop();
				enemyWaveTimes.pop();
			}
		}

		//Update enemies
		for (auto &i : enemies) {
			for (auto &j : i) {
				j.update(dt, *player);
				j.draw(window);
			}
		}

		player->update(dt);
		player->draw(window);
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
				i.y = - cloudTexH;
				i.x = xDist(mt);
			}
			cloudSprite.setPosition(i.x, i.y);
			window.draw(cloudSprite);
		}
	}
};
