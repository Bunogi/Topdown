#pragma once

#include <map>
#include <string>

#include "entity/entity.hpp"

//Holds everything we need for enemies, makes for neater code
namespace Enemy {
	enum class EnemyType : int {
		Goon = 1
	};

	extern const std::map<EnemyType, std::string> enemyNames;
	extern std::map<std::string, EnemyType> getReverseEnemyNames();

	class Enemy : public Entity {
	public:
		void draw(sf::RenderWindow& window);
		void update(float dt);
		Enemy(EnemyType setType, unsigned gridPos, float gridSpc);
		const EnemyType type;
		unsigned gridPosition;
		float gridSpace;
	};
};
