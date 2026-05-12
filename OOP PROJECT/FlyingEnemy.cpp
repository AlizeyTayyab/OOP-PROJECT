#include "FlyingEnemy.h"
#include <cmath>

FlyingEnemy::FlyingEnemy()
{
    hp = 80;
    maxHP = 80;
    speed = 100.f;
    currentPathIndex = 0;
    alive = true;
    reachedEnd = false;
    exitPoint = sf::Vector2f(0.f, 0.f);
}

void FlyingEnemy::setExitPoint(sf::Vector2f exit)
{
    exitPoint = exit; // store the exit tile position
}

void FlyingEnemy::update(float dt)
{
    if (!alive || reachedEnd) return;

    // fly straight toward exit, ignoring all waypoints
    sf::Vector2f dir = exitPoint - position;
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (dist < 5.f)
    {
        // reached the exit
        reachedEnd = true;
        alive = false;
        return;
    }

    dir.x /= dist;
    dir.y /= dist;
    position.x += dir.x * speed * dt;
    position.y += dir.y * speed * dt;
}

void FlyingEnemy::render(sf::RenderWindow& window)
{
    // cyan diamond shape — looks like it's flying
    sf::CircleShape shape(10.f, 4); // 4 sides = diamond
    shape.setFillColor(sf::Color::Cyan);
    shape.setRotation(45.f);
    shape.setPosition(position);
    window.draw(shape);
}