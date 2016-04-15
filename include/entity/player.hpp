#pragma once

#include "entity/entity.hpp"

class Player : public Entity {
	float damageInvulnTime; //Time to stay invulnerable and flicker
	float damageInvulnResetTime; //Time to reset damageInvulnTime to when damage is taken
	float flickerSpeed; //Speed per flicker
	float flickerTime; //Current flicker time
	bool isFlickering; //Are we currently transparent?

	float heartDistance;
	float heartWidth;
	float heartYPos;
	sf::Texture heartTexture;
	sf::RectangleShape heartRect;
public:
	Player();
	void draw(sf::RenderWindow& window);
	void update(float dt);
	void doDamage(float dt);
	const sf::RectangleShape& getRect();
};
