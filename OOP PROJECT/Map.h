#pragma once
#include <SFML/Graphics.hpp>

class Map
{
private:
    int tileSize;
    int width;
    int height;
    sf::Vector2f* pathPoints;
    int pathLength;


    int** grid;  // 0 = ground, 1 = path, 2 = start, 3 = end

    sf::RectangleShape** tiles;

public:
    Map(int w, int h, int tileSize);
    sf::Vector2f* getPathPoints();
    int getPathLength();
    int getGridValue(int x, int y);

    void createGrid();
    void createPath();

    void draw(sf::RenderWindow& window);

    ~Map();
};
