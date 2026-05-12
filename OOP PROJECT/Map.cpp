#include "Map.h"

// constructor — type 0 = beginner, type 1 = pro, type 2 = chad
Map::Map(int w, int h, int tSize, int type)
{
    width = w;
    height = h;
    tileSize = tSize;
    mapType = type;

    // allocate grid
    grid = new int* [height];
    for (int y = 0; y < height; y++)
    {
        grid[y] = new int[width];
        for (int x = 0; x < width; x++)
            grid[y][x] = 0;
    }

    // allocate tiles
    tiles = new sf::RectangleShape * [height];
    for (int y = 0; y < height; y++)
        tiles[y] = new sf::RectangleShape[width];

    createGrid();
    createPath();
}

// create background grid
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

// mark one tile as main path
void Map::markPath(int x, int y)
{
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    grid[y][x] = 1;
    tiles[y][x].setFillColor(sf::Color(75, 0, 130));
    path.push_back(sf::Vector2f(
        x * tileSize + tileSize / 2.f,
        y * tileSize + tileSize / 2.f
    ));
}

// mark one tile as the second (bottom) path
void Map::markPath2(int x, int y)
{
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    grid[y][x] = 1;
    tiles[y][x].setFillColor(sf::Color(120, 0, 80));
    path2.push_back(sf::Vector2f(
        x * tileSize + tileSize / 2.f,
        y * tileSize + tileSize / 2.f
    ));
}

void Map::createPath()
{
    path.clear();
    path2.clear();

    if (mapType == 0)
        createPathBeginner();
    else if (mapType == 1)
        createPathPro();
    else
        createPathChad();
}

// ── BEGINNER MAP ──────────────────────────────────────────────────────────
void Map::createPathBeginner()
{
    for (int x = 0; x <= 5; x++)  markPath(x, 2);
    for (int y = 2; y <= 7; y++)  markPath(5, y);
    for (int x = 5; x <= 14; x++) markPath(x, 7);
    for (int y = 7; y <= 12; y++) markPath(14, y);
    for (int x = 14; x <= 19; x++) markPath(x, 12);

    tiles[2][0].setFillColor(sf::Color::Red);
    tiles[12][19].setFillColor(sf::Color::Cyan);
    grid[2][0] = 2;
    grid[12][19] = 3;
}

// ── PRO MAP ───────────────────────────────────────────────────────────────
void Map::createPathPro()
{
    // TOP PATH
    for (int x = 0; x <= 4; x++)  markPath(x, 1);
    for (int y = 1; y <= 4; y++)  markPath(4, y);
    for (int x = 4; x <= 8; x++)  markPath(x, 4);
    for (int y = 4; y <= 7; y++)  markPath(8, y);
    for (int x = 8; x <= 11; x++) markPath(x, 7);

    // BOTTOM PATH
    for (int x = 0; x <= 4; x++)  markPath2(x, 13);
    for (int y = 13; y >= 10; y--) markPath2(4, y);
    for (int x = 4; x <= 8; x++)  markPath2(x, 10);
    for (int y = 10; y >= 7; y--) markPath2(8, y);
    for (int x = 8; x <= 11; x++) markPath2(x, 7);

    // SHARED EXIT
    for (int x = 11; x <= 19; x++)
    {
        if (grid[7][x] != 1) markPath(x, 7);
        path2.push_back(sf::Vector2f(
            x * tileSize + tileSize / 2.f,
            7 * tileSize + tileSize / 2.f));
    }

    tiles[1][0].setFillColor(sf::Color::Red);
    tiles[13][0].setFillColor(sf::Color(255, 140, 0));
    tiles[7][19].setFillColor(sf::Color::Cyan);
    grid[1][0] = 2;
    grid[13][0] = 2;
    grid[7][19] = 3;
}

// ── CHAD MAP — much tighter zigzag, more turns, paths cross near centre ──
//
//  TOP PATH  (row 0 entry):
//    enter row 0 → down col 3 → right row 5 → down col 7 → left row 9
//    → up col 3 (second pass near top) → right row 3 → down col 11
//    → right row 7 to merge at col 13
//
//  BOTTOM PATH (row 14 entry):
//    enter row 14 → up col 3 → right row 10 → up col 7 → right row 6
//    → down col 11 (crosses mid) → right row 8 → up col 13 → right row 7
//    to merge at col 13
//
//  SHARED EXIT: row 7, col 13 → col 19
//
void Map::createPathChad()
{
    // ── TOP PATH ─────────────────────────────────────────────────────────
    // A1  enter left → right on row 0
    for (int x = 0; x <= 3; x++)   markPath(x, 0);
    // A2  turn down col 3 → row 5
    for (int y = 0; y <= 5; y++)   markPath(3, y);
    // A3  go right on row 5
    for (int x = 3; x <= 7; x++)   markPath(x, 5);
    // A4  turn down col 7 → row 9
    for (int y = 5; y <= 9; y++)   markPath(7, y);
    // A5  go left on row 9
    for (int x = 7; x >= 2; x--)  markPath(x, 9);
    // A6  turn up col 2 → row 3
    for (int y = 9; y >= 3; y--)  markPath(2, y);
    // A7  go right on row 3
    for (int x = 2; x <= 11; x++)  markPath(x, 3);
    // A8  turn down col 11 → row 7
    for (int y = 3; y <= 7; y++)   markPath(11, y);
    // A9  right on row 7 to merge col 13
    for (int x = 11; x <= 13; x++) markPath(x, 7);

    // ── BOTTOM PATH ───────────────────────────────────────────────────────
    // B1  enter left → right on row 14
    for (int x = 0; x <= 3; x++)   markPath2(x, 14);
    // B2  turn up col 3 → row 10
    for (int y = 14; y >= 10; y--) markPath2(3, y);
    // B3  go right on row 10
    for (int x = 3; x <= 7; x++)   markPath2(x, 10);
    // B4  turn up col 7 → row 6
    for (int y = 10; y >= 6; y--) markPath2(7, y);
    // B5  go right on row 6
    for (int x = 7; x <= 11; x++)  markPath2(x, 6);
    // B6  turn down col 11 → row 8
    for (int y = 6; y <= 8; y++)   markPath2(11, y);
    // B7  go right on row 8 to col 13
    for (int x = 11; x <= 13; x++) markPath2(x, 8);
    // B8  turn up col 13 → row 7
    for (int y = 8; y >= 7; y--)  markPath2(13, y);

    // ── SHARED EXIT: row 7, col 13 → 19 ──────────────────────────────────
    for (int x = 13; x <= 19; x++)
    {
        if (grid[7][x] != 1) markPath(x, 7);
        path2.push_back(sf::Vector2f(
            x * tileSize + tileSize / 2.f,
            7 * tileSize + tileSize / 2.f));
    }

    // colour entries/exit
    tiles[0][0].setFillColor(sf::Color::Red);
    tiles[14][0].setFillColor(sf::Color(255, 140, 0));
    tiles[7][19].setFillColor(sf::Color::Cyan);

    grid[0][0] = 2;
    grid[14][0] = 2;
    grid[7][19] = 3;
}

// ── Accessors ─────────────────────────────────────────────────────────────
std::vector<sf::Vector2f> Map::getPath() { return path; }
std::vector<sf::Vector2f> Map::getPath2() { return path2; }

int Map::getGridValue(int x, int y)
{
    if (x < 0 || x >= width || y < 0 || y >= height) return -1;
    return grid[y][x];
}

void Map::draw(sf::RenderWindow& window)
{
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            window.draw(tiles[y][x]);
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