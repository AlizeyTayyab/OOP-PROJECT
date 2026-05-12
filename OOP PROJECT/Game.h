#pragma once
#include <iostream>

// Game class handles economy, waves, and player state
class Game {
private:
    int gold;
    int lives;
    int wave;
    int maxWaves;

public:
    // Constructor
    Game();

    // Economy functions
    void addGold(int amount);
    bool spendGold(int amount);

    // Wave system
    void nextWave();
    int getEnemyCountForWave() const;

    // Lives system
    void loseLife();

    // Getters
    int getGold() const;
    int getLives() const;
    int getWave() const;
    int getMaxWaves() const;

    // Game state checks
    bool isGameOver() const;
    bool isWon() const;

    // ── Operator overloading ──────────────────────────────────────

    // prefix ++ advances to next wave (same as nextWave())
    Game& operator++();

    // += adds gold directly — e.g. game += 50 gives 50 gold
    Game& operator+=(int amount);

    // -= removes lives directly — e.g. game -= 1 loses a life
    Game& operator-=(int amount);
    void reset();
    // << prints current game state to output stream
    friend std::ostream& operator<<(std::ostream& os, const Game& g);
};