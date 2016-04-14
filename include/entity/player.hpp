#pragma once

#include "entity/entity.hpp"

class Player : public Entity {
	float damageInvulnTime; //Time to stay invulnerable and flicker
	float damageInvulnResetTime; //Time to reset damageInvulnTime to when damage is taken
	float flickerSpeed; //Speed per flicker
	float flickerTime; //Current flicker time
public:
	Player();
	void draw(sf::RenderWindow& window);
	void update(float dt);
	void doDamage(float dt);
	const sf::RectangleShape& getRect();
};
