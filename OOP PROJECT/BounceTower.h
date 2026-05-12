
#ifndef BOUNCETOWER_H
#define BOUNCETOWER_H
#include "Tower.h"

// Bounce tower — shot bounces between up to 3 enemies in range
class BounceTower : public Tower
{
public:
    BounceTower(sf::Vector2f pos);
    void attack(std::vector<Enemy*>& enemies) override;
};
#endif