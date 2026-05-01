#include "Enemy.h"

Enemy::Enemy()
{
    speed = 0;
    currentPathIndex = 0;

    pathPoints = nullptr;
    pathLength = 0;

    hp = 100;      // ✅ FIX
    alive = true;  // safety
}


// Set full path for enemy movement
void Enemy::setPath(sf::Vector2f* points, int length)
{
    pathPoints = points;
    pathLength = length;
    currentPathIndex = 0;
}

// Place enemy at starting position
void Enemy::setStartPosition(sf::Vector2f start)
{
    position = start;
}

// Reduce HP when enemy is hit
void Enemy::takeDamage(int amount)
{
    hp -= amount;

    if (hp <= 0)
    {
        hp = 0;
        alive = false;   // mark dead
    }
}

// Check if enemy is dead
bool Enemy::isDead() const
{
    return !alive;
}

// Needed for tower targeting (range calculation)
sf::Vector2f Enemy::getPosition() const
{
    return position;
}
