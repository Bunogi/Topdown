//Base class for entities

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class Entity {
public:
	float x, y;
	float speed;
	virtual void draw(sf::RenderTexture& texture) = 0;
	virtual void update(float dt) = 0;
protected:
	sf::Texture entTexture;
	sf::RectangleShape rect;
};
