#include "SniperTower.h"
#include "Enemy.h"
#include <cmath>

SniperTower::SniperTower(sf::Vector2f pos)
    : Tower(pos, 350.f, 60, 2.0f) // long range, high damage, slow fire
{
    shape.setFillColor(sf::Color(0, 100, 255)); // bright blue
}

void SniperTower::attack(std::vector<Enemy*>& enemies)
{
    if (cooldown > 0.f) return;

    // target the enemy furthest along the path in range
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
        target->takeDamage(damage); // single precise shot
        cooldown = fireRate;
    }
}