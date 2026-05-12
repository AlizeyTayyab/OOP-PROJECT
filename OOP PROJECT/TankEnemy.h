#pragma once
#ifndef TANKENEMY_H
#define TANKENEMY_H
#include "Enemy.h"

// Tank enemy — very slow, extremely high HP
class TankEnemy : public Enemy
{
public:
    TankEnemy();
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
};
#endif