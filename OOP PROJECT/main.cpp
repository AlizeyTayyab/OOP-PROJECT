#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "Map.h"
#include "Enemy.h"
#include "BasicEnemy.h"
#include "Tower.h"
#include "CannonTower.h"

using namespace std;

int main()
{
    int tileSize = 40;
    int width = 20;
    int height = 15;

    // create window
    sf::RenderWindow window(
        sf::VideoMode(width * tileSize, height * tileSize),
        "Tower Defense Test"
    );

    int wave = 1; // wave counter

    // create map
    Map map(width, height, tileSize);

    // =========================
    // ENEMY SYSTEM
    // =========================

    int maxEnemies = 10;

    // enemy array (polymorphic base pointers)
    Enemy** enemies = new Enemy * [maxEnemies];

    int enemyCount = 3;     // enemies per wave
    int spawnedEnemies = 0; // spawned so far
    int enemiesAlive = 0;   // alive count

    int spawnTimer = 0;     // spawn timer
    int spawnDelay = 20;    // spawn delay

    // =========================
    // TOWER SYSTEM
    // =========================

    vector<Tower*> towers; // store all towers

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            // close window
            if (event.type == sf::Event::Closed)
                window.close();

            // mouse click to place tower
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                int x = mousePos.x / tileSize;
                int y = mousePos.y / tileSize;

                // check valid tile
                if (x >= 0 && x < width && y >= 0 && y < height)
                {
                    if (map.getGridValue(x, y) == 0)
                    {
                        sf::Vector2f towerPos(
                            x * tileSize + tileSize / 2,
                            y * tileSize + tileSize / 2
                        );

                        // create cannon tower
                        towers.push_back(new CannonTower(towerPos));
                    }
                }
            }
        }

        // =========================
        // SPAWN SYSTEM
        // =========================

        spawnTimer++;

        if (spawnedEnemies < enemyCount)
        {
            if (spawnTimer >= spawnDelay)
            {
                // create enemy (polymorphism)
                enemies[spawnedEnemies] = new BasicEnemy();

                // set path
                enemies[spawnedEnemies]->setPath(
                    map.getPathPoints(),
                    map.getPathLength()
                );

                // set start position
                enemies[spawnedEnemies]->setStartPosition(
                    map.getPathPoints()[0]
                );

                spawnedEnemies++;
                spawnTimer = 0;
            }
        }

        // =========================
        // UPDATE ENEMIES
        // =========================

        enemiesAlive = 0;

        for (int i = 0; i < spawnedEnemies; i++)
        {
            if (!enemies[i]->isDead())
            {
                enemies[i]->update();
                enemiesAlive++;
            }
        }

        // =========================
        // WAVE SYSTEM
        // =========================

        if (enemiesAlive == 0 && spawnedEnemies == enemyCount)
        {
            wave++; // next wave

            enemyCount++; // increase difficulty

            if (enemyCount > maxEnemies)
                enemyCount = maxEnemies;

            spawnedEnemies = 0;
            spawnTimer = 0;
            enemiesAlive = 0;
        }

        // =========================
        // TOWER ATTACK SYSTEM
        // =========================

        vector<Enemy*> enemyPointers;

        // collect alive enemies
        for (int i = 0; i < spawnedEnemies; i++)
        {
            if (!enemies[i]->isDead())
            {
                enemyPointers.push_back(enemies[i]);
            }
        }

        // towers attack enemies
        for (Tower* tower : towers)
        {
            tower->update(0.016f);        // cooldown update
            tower->attack(enemyPointers); // attack logic
        }

        // =========================
        // DRAW EVERYTHING
        // =========================

        window.clear(sf::Color::Red);

        // draw map
        map.draw(window);

        // draw enemies
        for (int i = 0; i < spawnedEnemies; i++)
        {
            if (!enemies[i]->isDead())
            {
                enemies[i]->render(window);
            }
        }

        // draw towers
        for (Tower* tower : towers)
        {
            tower->render(window);
        }

        window.display();
    }

    // =========================
    // CLEANUP
    // =========================

    for (int i = 0; i < spawnedEnemies; i++)
    {
        delete enemies[i];
    }

    delete[] enemies;

    for (Tower* tower : towers)
    {
        delete tower;
    }

    return 0;
}
