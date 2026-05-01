#include "CannonTower.h"
#include "Enemy.h"
#include <cmath>

CannonTower::CannonTower(sf::Vector2f pos)
    : Tower(pos, 150.f, 25, 1.0f)   // position, range, damage, fireRate
{
    shape.setFillColor(sf::Color::Red);
}

void CannonTower::attack(std::vector<Enemy*>& enemies)
{
    if (cooldown > 0)
        return;

    for (Enemy* enemy : enemies)
    {
        float dx = enemy->getPosition().x - position.x;
        float dy = enemy->getPosition().y - position.y;

        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= range)
        {
            enemy->takeDamage(damage);   // Hit enemy
            cooldown = fireRate;         // Reset cooldown
            break;                       // Attack only one enemy
        }
    }
}
