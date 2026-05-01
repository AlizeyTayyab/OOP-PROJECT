
#ifndef ENTITY_H
#define ENTITY_H
#include<iostream>
using namespace std;
#include <SFML/Graphics.hpp>
class Entity {
protected:
    sf::Vector2f position;
    int health;

public:
    Entity();

    virtual void update() = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    virtual ~Entity() {}
    void takeDamage(int dmg);
    bool isDead();
    sf::Vector2f getPosition();


};
#endif