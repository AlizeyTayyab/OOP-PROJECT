#pragma once
#ifndef STEALTHENEMY_H
#define STEALTHENEMY_H
#include "Enemy.h"

// Stealth enemy — periodically turns invisible, towers cannot target it
class StealthEnemy : public Enemy
{
private:
    bool  invisible;       // true = towers skip this enemy
    float invisTimer;      // counts time in current state
    float visibleDuration; // how long it stays visible
    float invisDuration;   // how long it stays invisible
public:
    StealthEnemy();
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    bool isInvisible() const; // towers check this before targeting
};
#endif