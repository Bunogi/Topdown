#include "entity/projectile.hpp"

Projectile::Projectile(float xPos, float yPos, float spd) {
	x = xPos;
	y = yPos;
	speed = spd;

	rect = sf::RectangleShape(sf::Vector2f(10.f, 10.f));
	sf::Vector2f size = rect.getSize();
	ySize = size.y;
	rect.setOrigin(size.x / 2.f, size.y / 2.f);
}

void Projectile::draw(sf::RenderWindow& window) {
	window.draw(rect);
}

void Projectile::update(float dt) {
	y -= speed * dt;
	rect.setPosition(x, y);
}

bool Projectile::shouldRemove() {
	return y < -ySize;
}
