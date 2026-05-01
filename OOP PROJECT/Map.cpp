#include "Map.h"

Map::Map(int w, int h, int tSize)
{
    width = w;
    height = h;
    tileSize = tSize;

    // allocate grid
    grid = new int* [height];

    for (int y = 0; y < height; y++)
    {
        grid[y] = new int[width];

        for (int x = 0; x < width; x++)
        {
            grid[y][x] = 0; // default = ground
        }
    }

    // allocate tiles
    tiles = new sf::RectangleShape * [height];

    for (int y = 0; y < height; y++)
    {
        tiles[y] = new sf::RectangleShape[width];
    }
    pathLength = width - 4;
    pathPoints = new sf::Vector2f[pathLength]; //we are storing the coordinates of each tile in an array of PathPoints using a built in sfml class


    createGrid();
    createPath();
}

void Map::createGrid()
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            tiles[y][x].setSize(sf::Vector2f(tileSize - 1, tileSize - 1));
            tiles[y][x].setPosition(x * tileSize, y * tileSize);

            tiles[y][x].setFillColor(sf::Color(204, 153, 255)); // ground
        }
    }
}

void Map::createPath()
{ 
    int midY = height / 2;

    int index = 0; // index for pathPoints array

    for (int x = 2; x < width - 2; x++)
    {
        // mark path in grid
        grid[midY][x] = 1;

        // color path tile
        tiles[midY][x].setFillColor(sf::Color(75, 0, 130));

        // store center position of this tile
        pathPoints[index] = sf::Vector2f(
            x * tileSize + tileSize / 2,
            midY * tileSize + tileSize / 2
        );

        index++;
    }

    // start point
    grid[midY][2] = 2;
    tiles[midY][2].setFillColor(sf::Color::Red);

    // end point
    grid[midY][width - 3] = 3;
    tiles[midY][width - 3].setFillColor(sf::Color::Cyan);
}
sf::Vector2f* Map::getPathPoints()
{
    return pathPoints;
}

int Map::getPathLength()
{
    return pathLength;
}


void Map::draw(sf::RenderWindow& window)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            window.draw(tiles[y][x]);
        }
    }
}
int Map::getGridValue(int x, int y)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return -1;

    return grid[y][x];
}


Map::~Map()
{
    for (int y = 0; y < height; y++)
    {
        delete[] grid[y];
        delete[] tiles[y];
    }

    delete[] grid;
    delete[] tiles;
}
