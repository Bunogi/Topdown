#include <iostream>
#include "game.hpp"
#include "resource.hpp"

namespace Game {
	//RNG for cloud positions
	std::random_device dev;
	std::mt19937 mt(dev());
	std::uniform_real_distribution<float> xDist;
	std::uniform_real_distribution<float> yDist; //Only used at the start
	std::uniform_real_distribution<float> speedDist;

	std::vector<Cloud> clouds;
	sf::Texture cloudTexture;
	sf::Sprite cloudSprite;
	sf::Vector2u windowSize;

	unsigned cloudCount = 0;
	const float cloudSpeed = 60.f;

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
