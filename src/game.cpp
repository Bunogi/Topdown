#include <iostream>
#include <vector>
#include <stack>
#include <list>

#include <libconfig.h++>

#include "game.hpp"
#include "entity/enemy.hpp"
#include "entity/player.hpp"
#include "entity/projectile.hpp"
#include "resource.hpp"
#include "configException.hpp"

namespace Game {
	//RNG for cloud positions
	std::random_device dev;
	std::mt19937 mt(dev());
	std::uniform_real_distribution<float> xDist;
	std::uniform_real_distribution<float> yDist; //Only used at the start
	std::uniform_int_distribution<> typeDist;

	std::vector<Cloud> clouds;
	sf::Texture cloudTexture;
	sf::Sprite cloudSprite;
	sf::Vector2u windowSize;

	std::list<Enemy::Enemy> enemies;
	std::stack<float> enemyWaveTimes;
	std::vector<Projectile> playerProjectiles;

	unsigned cloudCount = 0;
	const float cloudSpeed = 60.f;

	float totalTime = 0.f;

	float scrollSpeed = 0.f;
	float scroll = 0.f;

	float cloudTexH = 0.f;

	int waveSize = 0;
	float gridSpace = 0.f;

	Player *player = nullptr;

	void init(const sf::Vector2u winSize) {
		windowSize = winSize;
		xDist = std::uniform_real_distribution<float>(20.f, windowSize.x - 20.f); //Keep clouds off the edges of the screen
		yDist = std::uniform_real_distribution<float>(20.f, windowSize.x - 20.f);
		typeDist = std::uniform_int_distribution<>(1, static_cast<int>(Enemy::EnemyType::COUNT) - 1);
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
		Enemy::load();
	}

	void playerShoot() {
		playerProjectiles.emplace_back(Projectile(player->x, player->y - (player->getRect().getSize().y / 2.f), player->projectileSpeed));
	}

	void loadLevel(std::string level) {
		if (player != nullptr)
			delete player;
		player = new Player;

		playerProjectiles.clear();

		using namespace libconfig;
		try {
			Config config;
			config.readFile(level.c_str());
			const Setting& info = config.getRoot()["level"];
			const Setting& waves = info["waves"];

			cloudCount = info["clouds"];
			genClouds();
			scrollSpeed = info["scrollSpeed"];
			scroll = 0.0f;
			waveSize = info["waveSize"];

			gridSpace = windowSize.x / waveSize;

			enemies.clear();
			for (auto it = waves.end() - 1; it != waves.begin() - 1; it--) { //rbegin() and rend() don't exist in the libconfig iterators
				enemyWaveTimes.push(*it);
			}
		} CATCH_SETTING_ERRORS(level);
	}

	void generateWave() {
		/* TOOD: Add probabilities */
		for (unsigned i = 0; i < waveSize; i++)
			enemies.push_back(Enemy::Enemy(static_cast<Enemy::EnemyType>(typeDist(mt)), i, gridSpace));
	}

	void update(float dt, sf::RenderWindow& window) {
		totalTime += dt;
		scroll += scrollSpeed * dt;

		//Insert new enemies
		if (enemyWaveTimes.size() > 0) {
			if (scroll >= enemyWaveTimes.top())  { //Probably won't ever be equal to scroll. It's just too unlikely
				generateWave();
				enemyWaveTimes.pop();
			}
		}

		//Update enemies
		for (auto it = enemies.begin(); it != enemies.end(); ) {
			if (it->shouldDelete()) {
				it = enemies.erase(it); //Enemy is dead or off-screen
				continue;
			}
			for (auto it2 = playerProjectiles.begin(); it2 != playerProjectiles.end(); ){
				//Check collision with the enemy we're currently updating
				if (it2->getRect().getGlobalBounds().intersects(it->getRect().getGlobalBounds())) {
					it->doDamage();
					it2 = playerProjectiles.erase(it2);
					continue;
				}
				it2++;
			}
			it->update(dt, *player);
			it->draw(window);
			it++;
		}

		//Remove unnecesarry projectiles, update all projectiles
		for (auto it = playerProjectiles.begin(); it != playerProjectiles.end(); ) {
			if (it->shouldRemove()) {
				it = playerProjectiles.erase(it);
				continue;
			}
			it->update(dt);
			it->draw(window);
			it++;
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
