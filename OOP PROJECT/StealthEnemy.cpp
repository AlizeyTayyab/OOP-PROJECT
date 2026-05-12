#include "StealthEnemy.h"
#include <cmath>

StealthEnemy::StealthEnemy()
{
    hp = 120;
    maxHP = 120;
    speed = 90.f;
    currentPathIndex = 0;
    alive = true;
    reachedEnd = false;
    invisible = false;
    invisTimer = 0.f;
    visibleDuration = 3.f; // visible for 3 seconds
    invisDuration = 2.f; // invisible for 2 seconds
}

void StealthEnemy::update(float dt)
{
    if (!alive || reachedEnd) return;

    // toggle between visible and invisible states
    invisTimer += dt;
    if (!invisible && invisTimer >= visibleDuration)
    {
        invisible = true;  // go invisible
        invisTimer = 0.f;
    }
    else if (invisible && invisTimer >= invisDuration)
    {
        invisible = false; // reappear
        invisTimer = 0.f;
    }

    // move along path normally
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

void StealthEnemy::render(sf::RenderWindow& window)
{
    // draw semi-transparent when invisible, solid when visible
    sf::CircleShape shape(10.f);
    if (invisible)
        shape.setFillColor(sf::Color(128, 0, 128, 80)); // faded purple
    else
        shape.setFillColor(sf::Color(128, 0, 128));     // solid purple
    shape.setPosition(position);
    window.draw(shape);
}

bool StealthEnemy::isInvisible() const
{
    return invisible;
}