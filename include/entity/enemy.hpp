#pragma once

#include <map>
#include <string>

#include "entity/entity.hpp"
#include "entity/player.hpp"

//Holds everything we need for enemies, makes for neater code (in theory)
namespace Enemy {
	enum class EnemyType : int {
		Goon = 1,
		Diver = 2
	};

	extern const std::map<EnemyType, std::string> enemyNames;

	//Load the enemy textures
	void load();

	class Enemy : public Entity {
		int xSize, ySize;
		float xOffset, yOffset;
	public:
		void draw(sf::RenderWindow& window);
		void update(float);
		void update(float dt, Player& player);
		Enemy(EnemyType setType, unsigned gridPos, float gridSpc);
		const EnemyType type;
		unsigned gridPosition;
		float gridSpace;
		bool shouldDelete();
		void doDamage();
	};
};
