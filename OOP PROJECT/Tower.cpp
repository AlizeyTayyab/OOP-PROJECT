#include "Tower.h"

Tower::Tower(sf::Vector2f pos, float r, int dmg, float rate)
    : position(pos), range(r), damage(dmg), fireRate(rate), cooldown(0.f)
{
    shape.setRadius(20.f);
    shape.setFillColor(sf::Color::Blue);
    shape.setOrigin(20.f, 20.f);
    shape.setPosition(position);
}

void Tower::update(float deltaTime)
{
    if (cooldown > 0)
        cooldown -= deltaTime;
}

void Tower::render(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::Vector2f Tower::getPosition() const
{
    return position;
}

float Tower::getRange() const
{
    return range;
}
