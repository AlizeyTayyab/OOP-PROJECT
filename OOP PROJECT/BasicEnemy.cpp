#include "BasicEnemy.h"
#include <cmath>

BasicEnemy::BasicEnemy()
{
    hp = 100;
    maxHP = 100;
    speed = 80.f;
    baseSpeed = 80.f;
    currentPathIndex = 0;
    alive = true;
    reachedEnd = false;
}

void BasicEnemy::update(float dt)
{
    if (!alive || reachedEnd) return;

    updateSlow(dt); // tick slow timer, restore speed if expired

    float remaining = speed * dt;
    while (remaining > 0.f)
    {
        if (path.empty() || currentPathIndex >= (int)path.size())
        {
            reachedEnd = true; alive = false; return;
        }
        sf::Vector2f target = path[currentPathIndex];
        sf::Vector2f dir = target - position;
        float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (dist == 0.f) { currentPathIndex++; continue; }
        if (remaining >= dist)
        {
            position = target; remaining -= dist; currentPathIndex++;
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

void BasicEnemy::render(sf::RenderWindow& window)
{
    sf::CircleShape shape(10.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(position);
    window.draw(shape);
}