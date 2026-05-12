#include "TankEnemy.h"
#include <cmath>

TankEnemy::TankEnemy()
{
    hp = 400;      // very high HP — needs many hits
    maxHP = 400;
    speed = 35.f;     // very slow
    currentPathIndex = 0;
    alive = true;
    reachedEnd = false;
}

void TankEnemy::update(float dt)
{
    if (!alive || reachedEnd) return;

    float remaining = speed * dt;

    while (remaining > 0.f)
    {
        if (path.empty() || currentPathIndex >= (int)path.size())
        {
            reachedEnd = true;
            alive = false;
            return;
        }

        sf::Vector2f target = path[currentPathIndex];
        sf::Vector2f dir = target - position;
        float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        if (dist == 0.f) { currentPathIndex++; continue; }

        if (remaining >= dist)
        {
            position = target;
            remaining -= dist;
            currentPathIndex++;
        }
        else
        {
            dir.x /= dist; dir.y /= dist;
            position.x += dir.x * remaining;
            position.y += dir.y * remaining;
            remaining = 0.f;
        }
    }
}

void TankEnemy::render(sf::RenderWindow& window)
{
    // large dark rectangle — looks heavy and slow
    sf::RectangleShape shape(sf::Vector2f(22.f, 22.f));
    shape.setFillColor(sf::Color(100, 50, 0)); // dark brown
    shape.setPosition(position);
    window.draw(shape);
}