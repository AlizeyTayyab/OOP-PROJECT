#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Map
{
private:
    int tileSize;
    int width;
    int height;
    int mapType;                       // 0 = beginner, 1 = pro, 2 = chad
    int** grid;
    sf::RectangleShape** tiles;
    std::vector<sf::Vector2f> path;    // main path (beginner) or top path (pro/chad)
    std::vector<sf::Vector2f> path2;   // bottom path — only used in pro/chad map

    void markPath(int x, int y);       // helper — marks one tile as path
    void markPath2(int x, int y);      // helper — marks bottom path tile (pro/chad only)
    void createPathBeginner();         // original single winding path
    void createPathPro();              // two paths merging into one exit
    void createPathChad();             // three-segment nightmare: two paths, longer, more turns

public:
    Map(int w, int h, int tileSize, int type); // type: 0=beginner, 1=pro, 2=chad
    std::vector<sf::Vector2f> getPath();
    std::vector<sf::Vector2f> getPath2();      // returns bottom path for pro/chad map
    int getGridValue(int x, int y);
    void createGrid();
    void createPath();
    void draw(sf::RenderWindow& window);
    ~Map();
};