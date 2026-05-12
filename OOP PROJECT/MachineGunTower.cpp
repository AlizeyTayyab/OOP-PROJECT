#include "MachineGunTower.h"
#include "Enemy.h"
#include <cmath>

MachineGunTower::MachineGunTower(sf::Vector2f pos)
    : Tower(pos, 120.f, 8, 0.15f) // short range, low damage, very fast fire
{
    shape.setFillColor(sf::Color(200, 200, 0)); // dark yellow
}

void MachineGunTower::attack(std::vector<Enemy*>& enemies)
{
    if (cooldown > 0.f) return;

    // target leading enemy in range
    Enemy* target = nullptr;
    int    bestIndex = -1;

    for (Enemy* enemy : enemies)
    {
        if (!enemy || enemy->isDead()) continue;

        float dx = enemy->getPosition().x - position.x;
        float dy = enemy->getPosition().y - position.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist <= range && enemy->getPathIndex() > bestIndex)
        {
            bestIndex = enemy->getPathIndex();
            target = enemy;
        }
    }

    if (target)
    {
        target->takeDamage(damage); // rapid low damage shots
        cooldown = fireRate;
    }
}