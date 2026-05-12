#pragma once
#ifndef FASTENEMY_H
#define FASTENEMY_H
#include "Enemy.h"

// Fast enemy — high speed, very low HP
class FastEnemy : public Enemy
{
public:
    FastEnemy();
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
};
#endif