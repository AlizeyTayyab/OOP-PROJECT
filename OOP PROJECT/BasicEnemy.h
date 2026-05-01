
#ifndef BASICENEMY_H
#define BASICENEMY_H
#include<iostream>
using namespace std;
#include <SFML/Graphics.hpp>
#include"Enemy.h"
class BasicEnemy : public Enemy
{
public:
    BasicEnemy();

    void update() override;
    void render(sf::RenderWindow& window) override;
};

#endif