#include<iostream>
using namespace std;
#include"Entity.h";
Entity::Entity(){ //entity over here represents evry object in the game , that has some properties, can be updated or redrawn
	position = sf::Vector2f(0, 0); //where the object is on screen , like its position
	health = 100;// what is ts life value 
}
void Entity::takeDamage(int dmg)
{
    health -= dmg;
    if (health < 0) health = 0;
}

bool Entity::isDead()
{
    return health <= 0;
}

sf::Vector2f Entity::getPosition()
{
    return position;
}