#include "Entity.h"

Entity::Entity() {
    position = sf::Vector2f(0.f, 0.f);
}

sf::Vector2f Entity::getPosition() const {
    return position;
}
