#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include <SFML/Graphics.hpp>

class Enemy : public Entity
{
protected:
    float speed;                 // Movement speed of enemy
    int currentPathIndex;        // Current target path point

    sf::Vector2f* pathPoints;    // Path array
    int pathLength;              // Total path points
    int hp;

    bool alive;                  // Tracks if enemy is alive

public:
    Enemy();

    virtual void update() = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    virtual ~Enemy() {};

    void setPath(sf::Vector2f* points, int length);
    void setStartPosition(sf::Vector2f start);

    void takeDamage(int amount);         // Reduce HP when attacked
    bool isDead() const;                 // Check if enemy died

    sf::Vector2f getPosition() const;    // Needed for tower targeting
};

#endif
