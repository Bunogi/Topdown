#include <map>

#include "entity/entity.hpp"

//Holds everything we need for enemies, makes for neater code
namespace Enemy {
	enum class EnemyType : int {
		NoShoot
	};

	extern const std::map<EnemyType, const char *> enemyNames;

	class Enemy : public Entity {
	public:
		void draw(sf::RenderTexture& texture);
		void update(float dt);
		Enemy(EnemyType setType);
		const EnemyType type;
	};
};
