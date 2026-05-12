#pragma once
#ifndef SLOWTOWER_H
#define SLOWTOWER_H
#include "Tower.h"

// Slow tower — reduces speed of all enemies within range
class SlowTower : public Tower
{
public:
    SlowTower(sf::Vector2f pos);
    void attack(std::vector<Enemy*>& enemies) override;
};
#endif