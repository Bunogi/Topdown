#include <libconfig.h++>

#include "entity/enemy.hpp"
#include "resource.hpp"

namespace Enemy {
	using namespace libconfig;
	const std::map<EnemyType, std::string> enemyNames {
		{ EnemyType::NoShoot, "NoShoot" }
	};

	Enemy::Enemy(EnemyType setType) : type(setType) {
	}
};
