#pragma once
#ifndef FLYINGENEMY_H
#define FLYINGENEMY_H
#include "Enemy.h"

// Flying enemy — ignores the path, flies straight to the exit
class FlyingEnemy : public Enemy
{
private:
    sf::Vector2f exitPoint; // flies directly toward this
public:
    FlyingEnemy();
    void setExitPoint(sf::Vector2f exit); // call this after construction
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
};
#endif