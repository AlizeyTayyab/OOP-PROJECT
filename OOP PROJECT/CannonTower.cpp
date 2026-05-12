#include "CannonTower.h"
#include "Enemy.h"
#include <cmath>

CannonTower::CannonTower(sf::Vector2f pos)
    : Tower(pos, 150.f, 25, 1.0f)  // position, range, damage, fireRate
{
    shape.setFillColor(sf::Color::Red);
}

void CannonTower::attack(std::vector<Enemy*>& enemies)
{
    // cooldown not finished yet — wait
    if (cooldown > 0.f)
        return;

    // find the furthest-along enemy in range (highest path index)
    // targeting the leading enemy is better strategy than first in array
    Enemy* target = nullptr;
    int    bestIndex = -1;

    for (Enemy* enemy : enemies)
    {
        // skip dead or escaped enemies — they should not be in the list
        // but guard anyway so a stale pointer never gets hit
        if (!enemy || enemy->isDead())
            continue;

        float dx = enemy->getPosition().x - position.x;
        float dy = enemy->getPosition().y - position.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= range)
        {
            // prefer the enemy furthest along the path
            if (enemy->getPathIndex() > bestIndex)
            {
                bestIndex = enemy->getPathIndex();
                target = enemy;
            }
        }
    }

    if (target)
    {
        target->takeDamage(damage); // hit the leading enemy in range
        cooldown = fireRate;        // reset cooldown after firing
    }
}