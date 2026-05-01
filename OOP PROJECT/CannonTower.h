#pragma once

#include "Tower.h"

class CannonTower : public Tower
{
public:
    CannonTower(sf::Vector2f pos);

    void attack(std::vector<Enemy*>& enemies) override;
};
