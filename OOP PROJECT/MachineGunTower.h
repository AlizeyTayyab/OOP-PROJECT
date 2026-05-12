#pragma once
#ifndef MACHINEGUNTOWER_H
#define MACHINEGUNTOWER_H
#include "Tower.h"

// Machine gun tower — rapid fire, low damage per bullet
class MachineGunTower : public Tower
{
public:
    MachineGunTower(sf::Vector2f pos);
    void attack(std::vector<Enemy*>& enemies) override;
};
#endif