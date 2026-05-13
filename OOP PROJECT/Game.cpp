#include "Game.h"

// Constructor initializes starting values
Game::Game() {
    gold = 5000;  // starting money
    lives = 10;   // player health
    wave = 1;    // starting wave
    maxWaves = 5;    // 5 progressive waves total
}

// Add gold when enemy dies
void Game::addGold(int amount) {
    gold += amount;
}

// Spend gold for towers — returns false if not enough
bool Game::spendGold(int amount) {
    if (gold >= amount) {
        gold -= amount;
        return true;
    }
    return false;
}

// Move to next wave
void Game::nextWave() {
    wave++;
}

// Returns enemy count for current wave — scales up each wave
int Game::getEnemyCountForWave() const {
    // wave 1=3, wave 2=5, wave 3=7, wave 4=10, wave 5=14
    return 3 + (wave - 1) * 2 + (wave / 3);
}

// Lose a life when enemy reaches exit
void Game::loseLife() {
    lives--;
}

// Getters
int Game::getGold()     const { return gold; }
int Game::getLives()    const { return lives; }
int Game::getWave()     const { return wave; }
int Game::getMaxWaves() const { return maxWaves; }

// Game over when lives run out
bool Game::isGameOver() const { return lives <= 0; }

// Win when all waves cleared
bool Game::isWon() const { return wave > maxWaves; }



// prefix ++ — advance wave (e.g. ++game instead of game.nextWave())
Game& Game::operator++() {
    wave++;
    return *this;
}

// += — add gold directly (e.g. game += 10 on enemy kill)
Game& Game::operator+=(int amount) {
    gold += amount;
    return *this;
}

// -= — lose lives directly (e.g. game -= 1 when enemy escapes)
Game& Game::operator-=(int amount) {
    lives -= amount;
    if (lives < 0) lives = 0;
    return *this;
}
void Game::reset() {
    gold = 5000;
    lives = 10;
    wave = 1;
    maxWaves = 5;
}
// << — print game state (useful for debugging)
std::ostream& operator<<(std::ostream& os, const Game& g) {
    os << "Wave: " << g.wave << "/" << g.maxWaves
        << " | Gold: " << g.gold
        << " | Lives: " << g.lives;
    return os;
}