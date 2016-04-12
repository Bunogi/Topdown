#include <iostream>

#include <SFML/Graphics.hpp>

#include "entity/player.hpp"
#include "game.hpp"
#include "resource.hpp"

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "topdown", sf::Style::Titlebar);
	window.setVerticalSyncEnabled(true);

	sf::Event event;
	sf::Clock deltaClock;

	try {
		Game::init(window.getSize());
		Game::cloudCount = 5;
		Game::genClouds();
		Game::loadLevel(getResourcePath() + "/levels/one.cfg");
		Player player;

		while (window.isOpen()) {
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
			}

			float deltaTime = deltaClock.restart().asSeconds();
			player.update(deltaTime);

			window.clear(sf::Color(0x1C, 0x6B, 0xA0));
			Game::drawClouds(window, deltaTime);
			player.draw(window);
			window.display();
		}
	} catch(int i) { //A fatal, unrecoverable error has occured
		std::cerr << "Fatal error, terminating.\n";
		return i;
	}
}
