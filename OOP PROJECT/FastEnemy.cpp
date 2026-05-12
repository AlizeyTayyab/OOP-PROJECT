#include "FastEnemy.h"
#include <cmath>

FastEnemy::FastEnemy()
{
    hp = 40;       // very low HP — dies in 2 cannon shots
    maxHP = 40;
    speed = 160.f;    // twice as fast as BasicEnemy
    currentPathIndex = 0;
    alive = true;
    reachedEnd = false;
}

void FastEnemy::update(float dt)
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

void FastEnemy::render(sf::RenderWindow& window)
{
    // yellow triangle-ish circle — visually distinct from basic
    sf::CircleShape shape(8.f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setPosition(position);
    window.draw(shape);
}