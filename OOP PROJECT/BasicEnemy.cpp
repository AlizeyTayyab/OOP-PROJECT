#include <iostream>
#include "BasicEnemy.h"
#include <cmath>

BasicEnemy::BasicEnemy()
{
    // initial spawn position
    position = sf::Vector2f(0, 0);

    health = 100;
    speed = 2.0f;
    currentPathIndex = 0;
}

void BasicEnemy::update()
{
    // stop if no path or finished path
    if (!pathPoints || currentPathIndex >= pathLength)
        return;

    // current target point in path
    sf::Vector2f target = pathPoints[currentPathIndex];

    // direction vector (target - current position)
    sf::Vector2f direction = target - position;

    // distance to target
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance == 0)
        return;

    // if close enough, snap to target and move to next point
    if (distance < speed)
    {
        position = target;
        currentPathIndex++;
    }
    else
    {
        // normalize direction
        direction.x /= distance;
        direction.y /= distance;

        // move enemy
        position.x += direction.x * speed;
        position.y += direction.y * speed;
    }
}

void BasicEnemy::render(sf::RenderWindow& window)
{
    sf::CircleShape shape(10);
    shape.setFillColor(sf::Color::Green);

    // draw at current position
    shape.setPosition(position);

    window.draw(shape);
}

// NEW: allow external access to enemy position
