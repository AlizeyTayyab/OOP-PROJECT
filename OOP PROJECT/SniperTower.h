#pragma once
#ifndef SNIPERTOWER_H
#define SNIPERTOWER_H
#include "Tower.h"

// Sniper tower — very long range, high damage, slow fire rate
class SniperTower : public Tower
{
public:
    SniperTower(sf::Vector2f pos);
    void attack(std::vector<Enemy*>& enemies) override;
};
#endif