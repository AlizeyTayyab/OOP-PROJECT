#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Enemy; // Forward declaration

class Tower
{
protected:
    sf::Vector2f position;
    float range;
    int damage;
    float fireRate;
    float cooldown;

    sf::CircleShape shape;

public:
    Tower(sf::Vector2f pos, float r, int dmg, float rate);
    virtual ~Tower() = default;

    virtual void attack(std::vector<Enemy*>& enemies) = 0;

    virtual void update(float deltaTime);
    virtual void render(sf::RenderWindow& window);

    sf::Vector2f getPosition() const;
    float getRange() const;
};
