#include "entity/entity.hpp"

class Player : public Entity {
public:
	Player();
	void draw(sf::RenderTexture& texture);
	void update(float dt);
};
