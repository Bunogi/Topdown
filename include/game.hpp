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

	//Cloud stuff
	extern void genClouds();
	extern void drawClouds(sf::RenderWindow& window, float dt);
	extern unsigned cloudCount;

	extern sf::Vector2u windowSize;
};
