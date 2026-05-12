#include "Enemy.h"

Enemy::Enemy()
{
    currentPathIndex = 0;
    hp = 100;
    maxHP = 100;
    speed = 80.f;
    baseSpeed = 80.f;
    slowTimer = 0.f;
    alive = true;
    reachedEnd = false;
}

void Enemy::setPath(const std::vector<sf::Vector2f>& points)
{
    path = points;
}

void Enemy::setStartPosition(sf::Vector2f start)
{
    position = start;
    currentPathIndex = 0;
}

void Enemy::takeDamage(int amount)
{
    hp -= amount;
    if (hp <= 0) { hp = 0; alive = false; }
}

// apply a speed reduction for a given duration
void Enemy::applySlow(float speedMultiplier, float duration)
{
    speed = baseSpeed * speedMultiplier; // reduce speed
    slowTimer = duration;                    // reset slow timer
}

// tick slow timer and restore speed when it expires
void Enemy::updateSlow(float dt)
{
    if (slowTimer > 0.f)
    {
        slowTimer -= dt;
        if (slowTimer <= 0.f)
        {
            slowTimer = 0.f;
            speed = baseSpeed; // restore original speed
        }
    }
}

bool Enemy::isDead() const { return !alive || reachedEnd; }
bool Enemy::hasReachedEnd() const { return reachedEnd; }
int  Enemy::getHP() const { return hp; }
int  Enemy::getMaxHP() const { return maxHP; }
int  Enemy::getPathIndex() const { return currentPathIndex; }
sf::Vector2f Enemy::getPosition() const { return position; }