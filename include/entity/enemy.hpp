#pragma once

#include <map>

#include "entity/entity.hpp"

//Holds everything we need for enemies, makes for neater code
namespace Enemy {
	enum class EnemyType : int {
		Goon
	};

	extern const std::map<EnemyType, const char*> enemyNames;

	class Enemy : public Entity {
	public:
		void draw(sf::RenderWindow& window);
		void update(float dt);
		Enemy(EnemyType setType, unsigned gridPos);
		const EnemyType type;
		unsigned gridPosition;
		float gridSpace;
	};
};
