#include "BounceTower.h"
#include "Enemy.h"
#include <cmath>
#include <vector>

BounceTower::BounceTower(sf::Vector2f pos)
    : Tower(pos, 140.f, 20, 1.2f) // medium range, medium damage, medium fire
{
    shape.setFillColor(sf::Color(255, 100, 0)); // orange
}

void BounceTower::attack(std::vector<Enemy*>& enemies)
{
    if (cooldown > 0.f) return;

    // collect all enemies in range
    std::vector<Enemy*> inRange;
    for (Enemy* enemy : enemies)
    {
        if (!enemy || enemy->isDead()) continue;

        float dx = enemy->getPosition().x - position.x;
        float dy = enemy->getPosition().y - position.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist <= range)
            inRange.push_back(enemy);
    }

    if (inRange.empty()) return;

    // bounce shot hits up to 3 enemies, each for full damage
    int bounces = 0;
    for (Enemy* enemy : inRange)
    {
        if (bounces >= 3) break; // max 3 bounces
        enemy->takeDamage(damage);
        bounces++;
    }

    cooldown = fireRate;
}