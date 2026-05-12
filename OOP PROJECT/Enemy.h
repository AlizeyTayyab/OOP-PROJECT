#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Enemy : public Entity
{
protected:
    float speed;
    float baseSpeed;      // original speed before slow
    float slowTimer;      // counts down remaining slow duration
    int currentPathIndex;
    std::vector<sf::Vector2f> path;
    int hp;
    int maxHP;
    bool alive;
    bool reachedEnd;

public:
    Enemy();
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual ~Enemy() {}

    void setPath(const std::vector<sf::Vector2f>& points);
    void setStartPosition(sf::Vector2f start);
    void takeDamage(int amount);
    void applySlow(float speedMultiplier, float duration); // used by SlowTower

    bool isDead() const;
    bool hasReachedEnd() const;
    int  getHP() const;
    int  getMaxHP() const;
    int  getPathIndex() const;
    sf::Vector2f getPosition() const;

protected:
    void updateSlow(float dt); // call this in each subclass update
};
#endif