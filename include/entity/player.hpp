#include "entity/entity.hpp"

class Player : public Entity {
public:
	Player();
	void draw(sf::RenderWindow& window);
	void update(float dt);
};
