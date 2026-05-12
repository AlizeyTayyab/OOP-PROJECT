#include "SlowTower.h"
#include "Enemy.h"
#include <cmath>

SlowTower::SlowTower(sf::Vector2f pos)
    : Tower(pos, 130.f, 0, 0.5f) // range, no damage, applies slow frequently
{
    shape.setFillColor(sf::Color(0, 200, 200)); // teal
}

void SlowTower::attack(std::vector<Enemy*>& enemies)
{
    if (cooldown > 0.f) return;

    bool hit = false;

    // apply slow to ALL enemies in range, not just one
    for (Enemy* enemy : enemies)
    {
        if (!enemy || enemy->isDead()) continue;

        float dx = enemy->getPosition().x - position.x;
        float dy = enemy->getPosition().y - position.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist <= range)
        {
            enemy->applySlow(0.4f, 2.0f); // slow to 40% speed for 2 seconds
            hit = true;
        }
    }

    if (hit) cooldown = fireRate;
}