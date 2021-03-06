#pragma once

#include <random>
#include <vector>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace Game {
	struct Cloud {
		float x, y;
	};

	//Initialize RNG, clouds, load cloud texture
	extern void init(const sf::Vector2u winSize);

	//Update game state on a frame by frame basis
	extern void update(float dt, sf::RenderWindow& window);

	//Cloud stuff
	extern void genClouds();
	extern void drawClouds(sf::RenderWindow& window, float dt);
	extern unsigned cloudCount;

	//Load level
	extern void loadLevel(std::string level);

	extern sf::Vector2u windowSize;

	extern float scrollSpeed;
	extern float totalTime;

	extern void playerShoot();
};
