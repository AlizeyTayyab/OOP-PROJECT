#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

class Entity
{
protected:
    sf::Vector2f position;

public:
    Entity();
    virtual ~Entity() {}

    sf::Vector2f getPosition() const;
};

#endif
