#pragma once

#include "entity/entity.hpp"

class Projectile : public Entity {
	float ySize;
public:
	void draw(sf::RenderWindow& window);
	void update(float dt);
	Projectile(float x, float y, float speed);
	bool shouldRemove();
};
