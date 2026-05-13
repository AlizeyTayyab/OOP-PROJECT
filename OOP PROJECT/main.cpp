#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cmath>

#include "Map.h"
#include "Enemy.h"
#include "BasicEnemy.h"
#include "FastEnemy.h"
#include "TankEnemy.h"
#include "FlyingEnemy.h"
#include "StealthEnemy.h"
#include "Tower.h"
#include "CannonTower.h"
#include "SniperTower.h"
#include "MachineGunTower.h"
#include "SlowTower.h"
#include "BounceTower.h"
#include "Game.h"

using namespace std;

// ── Palette ───────────────────────────────────────────────────────────────
const sf::Color COL_VOID(8, 5, 15);
const sf::Color COL_DEEP_VIOLET(35, 15, 65);
const sf::Color COL_DUSK_PURPLE(80, 40, 140);
const sf::Color COL_LAVENDER(140, 100, 200);
const sf::Color COL_SUNSET_PINK(200, 80, 160);
const sf::Color COL_ROSE_GOLD(220, 130, 100);
const sf::Color COL_PALE_ROSE(240, 190, 200);
const sf::Color COL_CHAD_RED(255, 60, 30);
const sf::Color COL_CHAD_DARK(50, 8, 8);

// ── Tower costs ───────────────────────────────────────────────────────────
const int COST_CANNON = 1000;
const int COST_SNIPER = 1500;
const int COST_MACHGUN = 800;
const int COST_SLOW = 900;
const int COST_BOUNCE = 1200;

// ── Game states ───────────────────────────────────────────────────────────
const int STATE_MENU = 0;
const int STATE_PLAYING = 1;

const int MAX_ENEMIES = 100;

const float OUCH_MIN_INTERVAL = 0.25f;

// ─────────────────────────────────────────────────────────────────────────
Enemy* spawnEnemy(int wave, int index, sf::Vector2f exitPoint)
{
    if (wave == 1) return new BasicEnemy();

    if (wave == 2)
    {
        if (index % 3 == 0) return new FastEnemy();
        return new BasicEnemy();
    }
    if (wave == 3)
    {
        if (index % 4 == 0) return new TankEnemy();
        if (index % 4 == 1) return new FastEnemy();
        return new BasicEnemy();
    }
    if (wave == 4)
    {
        if (index % 5 == 0) { FlyingEnemy* f = new FlyingEnemy(); f->setExitPoint(exitPoint); return f; }
        if (index % 5 == 1) return new TankEnemy();
        if (index % 5 == 2) return new FastEnemy();
        return new BasicEnemy();
    }
    // wave 5+
    if (index % 6 == 0) { FlyingEnemy* f = new FlyingEnemy(); f->setExitPoint(exitPoint); return f; }
    if (index % 6 == 1) return new StealthEnemy();
    if (index % 6 == 2) return new TankEnemy();
    if (index % 6 == 3) return new FastEnemy();
    return new BasicEnemy();
}

int getCost(int sel)
{
    if (sel == 0) return COST_CANNON;
    if (sel == 1) return COST_SNIPER;
    if (sel == 2) return COST_MACHGUN;
    if (sel == 3) return COST_SLOW;
    return COST_BOUNCE;
}

Tower* buildTower(int sel, sf::Vector2f pos)
{
    if (sel == 0) return new CannonTower(pos);
    if (sel == 1) return new SniperTower(pos);
    if (sel == 2) return new MachineGunTower(pos);
    if (sel == 3) return new SlowTower(pos);
    return new BounceTower(pos);
}

void cleanEnemies(Enemy* enemies[], bool goldAwarded[], int count)
{
    for (int i = 0; i < count; i++)
    {
        delete enemies[i];
        enemies[i] = nullptr;
        goldAwarded[i] = false;
    }
}

void resetGame(Game& game,
    Enemy* enemies[], bool goldAwarded[], int maxEnemies,
    int& spawnedCount, int& enemyCount,
    float& spawnTimer, bool& waveInProgress,
    vector<Tower*>& towers)
{
    cleanEnemies(enemies, goldAwarded, maxEnemies);
    for (Tower* t : towers) delete t;
    towers.clear();
    game.reset();
    enemyCount = game.getEnemyCountForWave();
    spawnedCount = 0;
    spawnTimer = 0.f;
    waveInProgress = true;
}

void drawTowerShape(sf::RenderWindow& window, sf::Vector2f pos, int sel,
    const sf::Color& accent)
{
    const float R = 14.f;

    if (sel == 0)
    {
        sf::ConvexShape diamond;
        diamond.setPointCount(4);
        diamond.setPoint(0, { 0, -R });
        diamond.setPoint(1, { R,  0 });
        diamond.setPoint(2, { 0,  R });
        diamond.setPoint(3, { -R, 0 });
        diamond.setFillColor(accent);
        diamond.setOutlineColor(sf::Color::White);
        diamond.setOutlineThickness(1.5f);
        diamond.setPosition(pos);
        window.draw(diamond);

        sf::CircleShape core(4.f);
        core.setOrigin(4.f, 4.f);
        core.setFillColor(sf::Color::White);
        core.setPosition(pos);
        window.draw(core);
    }
    else if (sel == 1)
    {
        const float arm = R + 2.f, thick = 4.f;
        sf::RectangleShape h({ arm * 2, thick });
        h.setOrigin(arm, thick / 2.f);
        h.setFillColor(accent);
        h.setOutlineColor(sf::Color::White);
        h.setOutlineThickness(1.f);
        h.setPosition(pos);
        window.draw(h);

        sf::RectangleShape v({ thick, arm * 2 });
        v.setOrigin(thick / 2.f, arm);
        v.setFillColor(accent);
        v.setOutlineColor(sf::Color::White);
        v.setOutlineThickness(1.f);
        v.setPosition(pos);
        window.draw(v);

        sf::CircleShape dot(3.5f);
        dot.setOrigin(3.5f, 3.5f);
        dot.setFillColor(sf::Color::White);
        dot.setPosition(pos);
        window.draw(dot);
    }
    else if (sel == 2)
    {
        for (int i = -1; i <= 1; i++)
        {
            sf::RectangleShape bar({ R * 2.f, 5.f });
            bar.setOrigin(R, 2.5f);
            bar.setFillColor(i == 0 ? sf::Color::White : accent);
            bar.setOutlineColor(accent);
            bar.setOutlineThickness(1.f);
            bar.setPosition(pos.x, pos.y + i * 7.f);
            window.draw(bar);
        }
    }
    else if (sel == 3)
    {
        sf::ConvexShape hex;
        hex.setPointCount(6);
        for (int i = 0; i < 6; i++)
        {
            float angle = i * 60.f * 3.14159265f / 180.f;
            hex.setPoint(i, { R * std::cos(angle), R * std::sin(angle) });
        }
        hex.setFillColor(accent);
        hex.setOutlineColor(sf::Color::White);
        hex.setOutlineThickness(1.5f);
        hex.setPosition(pos);
        window.draw(hex);

        sf::CircleShape inner(5.f);
        inner.setOrigin(5.f, 5.f);
        inner.setFillColor(sf::Color(200, 240, 255, 200));
        inner.setPosition(pos);
        window.draw(inner);
    }
    else
    {
        sf::ConvexShape star;
        star.setPointCount(8);
        for (int i = 0; i < 8; i++)
        {
            float angle = i * 45.f * 3.14159265f / 180.f;
            float radius = (i % 2 == 0) ? R : R * 0.45f;
            star.setPoint(i, { radius * std::cos(angle), radius * std::sin(angle) });
        }
        star.setFillColor(accent);
        star.setOutlineColor(sf::Color::White);
        star.setOutlineThickness(1.f);
        star.setPosition(pos);
        window.draw(star);
    }
}


int main()
{
    const int tileSize = 40;
    const int mapWidth = 20;
    const int mapHeight = 15;
    const int panelW = 240;
    const int winW = mapWidth * tileSize + panelW;
    const int winH = mapHeight * tileSize + 40;

    // ── Create window and IMMEDIATELY paint dark — kills white flash ──────
    sf::RenderWindow window(sf::VideoMode(winW, winH), "Fatal Exception");
    window.setFramerateLimit(60);
    window.clear(COL_VOID);
    window.display();

    // ── Font ──────────────────────────────────────────────────────────────
    sf::Font font;
    if (!font.loadFromFile("Roboto_Condensed-Bold.ttf"))
    {
        cerr << "ERROR: font not found\n";
        return -1;
    }

    // ── Loading screen helper — called between each heavy asset block ─────
    auto drawLoadingScreen = [&](const string& msg)
        {
            window.clear(COL_VOID);

            sf::Text title;
            title.setFont(font);
            title.setString("Fatal Exception");
            title.setCharacterSize(38);
            title.setFillColor(COL_ROSE_GOLD);
            {
                sf::FloatRect b = title.getLocalBounds();
                title.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
                title.setPosition(winW / 2.f, winH / 2.f - 60.f);
            }

            sf::RectangleShape rule(sf::Vector2f(320.f, 2.f));
            rule.setFillColor(COL_DUSK_PURPLE);
            rule.setOrigin(160.f, 1.f);
            rule.setPosition(winW / 2.f, winH / 2.f - 14.f);

            sf::Text info;
            info.setFont(font);
            info.setString(msg);
            info.setCharacterSize(16);
            info.setFillColor(COL_LAVENDER);
            {
                sf::FloatRect b = info.getLocalBounds();
                info.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
                info.setPosition(winW / 2.f, winH / 2.f + 10.f);
            }

            window.draw(title);
            window.draw(rule);
            window.draw(info);
            window.display();
        };

    // ── Load Start music only — defer Laser/Win/Lose/Ouch to game start ──
    drawLoadingScreen("Loading audio...");

    sf::Music musStart, musLaser, musLose, musWin, musOuch;
    bool hasStart = musStart.openFromFile(
        "C:/Users/Alizey/source/repos/OOP PROJECT/x64/Debug/Assets/Sounds/Start.ogg");

    // These are loaded lazily inside startGame lambda below
    bool hasLaser = false;
    bool hasLose = false;
    bool hasWin = false;
    bool hasOuch = false;

    if (!hasStart) cerr << "WARNING: Start.ogg not found\n";
    if (hasStart)  musStart.play();

    auto playMusic = [](sf::Music& m, bool has) { if (has) m.play(); };
    auto stopMusic = [](sf::Music& m, bool has) { if (has) m.stop(); };

    bool  soundEndPlayed = false;
    float ouchCooldown = 0.f;

    // ── Enemy textures ────────
    drawLoadingScreen("Loading enemy sprites...");

    const string ENEMY_ASSET_PATH =
        "C:/Users/Alizey/source/repos/OOP PROJECT/x64/Debug/Assets/Enemies/";

    sf::Texture enemyTex[5];
    bool        enemyTexOk[5] = {};
    for (int i = 0; i < 5; i++)
    {
        enemyTexOk[i] = enemyTex[i].loadFromFile(
            ENEMY_ASSET_PATH + "E" + to_string(i + 1) + ".png");
        if (!enemyTexOk[i])
            cerr << "WARNING: E" << (i + 1) << ".png not found\n";
        else
            enemyTex[i].setSmooth(true);
    }

    // ── Tower textures ──────────────────────
    drawLoadingScreen("Loading tower sprites...");

    const string TOWER_ASSET_PATH =
        "C:/Users/Alizey/source/repos/OOP PROJECT/x64/Debug/Assets/Towers/";

    sf::Texture towerTex[5];
    bool        towerTexOk[5] = {};
    for (int i = 0; i < 5; i++)
    {
        towerTexOk[i] = towerTex[i].loadFromFile(
            TOWER_ASSET_PATH + "T" + to_string(i + 1) + ".png");
        if (!towerTexOk[i])
            cerr << "WARNING: T" << (i + 1) << ".png not found\n";
        else
            towerTex[i].setSmooth(true);
    }

    // ── Menu background ───────────────
    drawLoadingScreen("Loading menu...");

    sf::Texture menuTexture;
    if (!menuTexture.loadFromFile(
        "C:/Users/Alizey/source/repos/OOP PROJECT/OOP PROJECT/Assets/Background/Knight.jpg"))
        cerr << "ERROR: menu background not found\n";

    // ── Type-index helpers ─────────────
    auto enemyTexIndex = [](Enemy* e) -> int
        {
            if (dynamic_cast<StealthEnemy*>(e)) return 4;
            if (dynamic_cast<FlyingEnemy*>(e))  return 3;
            if (dynamic_cast<TankEnemy*>(e))    return 2;
            if (dynamic_cast<FastEnemy*>(e))    return 1;
            return 0;
        };

    auto towerTexIndex = [](Tower* t) -> int
        {
            if (dynamic_cast<CannonTower*>(t))     return 0;
            if (dynamic_cast<SniperTower*>(t))     return 1;
            if (dynamic_cast<MachineGunTower*>(t)) return 2;
            if (dynamic_cast<SlowTower*>(t))       return 3;
            return 4;
        };

    // ===========================
    // MENU UI
    // =================
    int gameState = STATE_MENU;
    int selectedMap = 0;

    sf::Sprite menuSprite;
    menuSprite.setTexture(menuTexture);
    menuSprite.setScale(
        (float)winW / menuTexture.getSize().x,
        (float)winH / menuTexture.getSize().y);

    sf::RectangleShape darkOverlay(sf::Vector2f(winW, winH));
    darkOverlay.setFillColor(sf::Color(0, 0, 0, 140));

    auto centerText = [](sf::Text& t, float x, float y)
        {
            sf::FloatRect b = t.getLocalBounds();
            t.setOrigin(b.left + b.width / 2.f, b.top);
            t.setPosition(x, y);
        };

    sf::Text menuTitle;
    menuTitle.setFont(font);
    menuTitle.setString("Fatal Exception");
    menuTitle.setCharacterSize(52);
    menuTitle.setFillColor(sf::Color::Black);
    centerText(menuTitle, winW / 2.f, 55.f);

    sf::RectangleShape titleUnderline(sf::Vector2f(460, 2));
    titleUnderline.setFillColor(COL_LAVENDER);
    titleUnderline.setOrigin(230, 0);
    titleUnderline.setPosition(winW / 2.f, 118.f);

    sf::Text menuSub;
    menuSub.setFont(font);
    menuSub.setString("Select Your Battlefield");
    menuSub.setCharacterSize(20);
    menuSub.setFillColor(COL_LAVENDER);
    centerText(menuSub, winW / 2.f, 128.f);

    const float BTN_MENU_W = 260.f;
    const float BTN_MENU_H = 60.f;
    const float BTN_MENU_X = winW / 2.f;

    auto centreInBox = [&](sf::Text& t, float bx, float by, float bw, float bh)
        {
            sf::FloatRect b = t.getLocalBounds();
            t.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
            t.setPosition(bx + bw / 2.f, by + bh / 2.f);
        };

    const float BOX_Y_NOOB = 220.f;
    sf::RectangleShape btnBeginner(sf::Vector2f(BTN_MENU_W, BTN_MENU_H));
    btnBeginner.setFillColor(COL_DEEP_VIOLET);
    btnBeginner.setOutlineThickness(2.f);
    btnBeginner.setOutlineColor(COL_DUSK_PURPLE);
    btnBeginner.setOrigin(BTN_MENU_W / 2.f, 0);
    btnBeginner.setPosition(BTN_MENU_X, BOX_Y_NOOB);

    sf::Text btnBeginnerTitle;
    btnBeginnerTitle.setFont(font);
    btnBeginnerTitle.setString("NOOB");
    btnBeginnerTitle.setCharacterSize(28);
    btnBeginnerTitle.setFillColor(COL_LAVENDER);
    centreInBox(btnBeginnerTitle, BTN_MENU_X - BTN_MENU_W / 2.f, BOX_Y_NOOB, BTN_MENU_W, BTN_MENU_H);

    const float BOX_Y_PRO = 310.f;
    sf::RectangleShape btnPro(sf::Vector2f(BTN_MENU_W, BTN_MENU_H));
    btnPro.setFillColor(sf::Color(50, 12, 35));
    btnPro.setOutlineThickness(2.f);
    btnPro.setOutlineColor(COL_SUNSET_PINK);
    btnPro.setOrigin(BTN_MENU_W / 2.f, 0);
    btnPro.setPosition(BTN_MENU_X, BOX_Y_PRO);

    sf::Text btnProTitle;
    btnProTitle.setFont(font);
    btnProTitle.setString("LEGEND");
    btnProTitle.setCharacterSize(28);
    btnProTitle.setFillColor(COL_SUNSET_PINK);
    centreInBox(btnProTitle, BTN_MENU_X - BTN_MENU_W / 2.f, BOX_Y_PRO, BTN_MENU_W, BTN_MENU_H);

    const float BOX_Y_CHAD = 400.f;
    sf::RectangleShape btnChad(sf::Vector2f(BTN_MENU_W, BTN_MENU_H));
    btnChad.setFillColor(COL_CHAD_DARK);
    btnChad.setOutlineThickness(2.f);
    btnChad.setOutlineColor(COL_CHAD_RED);
    btnChad.setOrigin(BTN_MENU_W / 2.f, 0);
    btnChad.setPosition(BTN_MENU_X, BOX_Y_CHAD);

    sf::Text btnChadTitle;
    btnChadTitle.setFont(font);
    btnChadTitle.setString("CHAD");
    btnChadTitle.setCharacterSize(28);
    btnChadTitle.setFillColor(COL_CHAD_RED);
    centreInBox(btnChadTitle, BTN_MENU_X - BTN_MENU_W / 2.f, BOX_Y_CHAD, BTN_MENU_W, BTN_MENU_H);

    sf::Text menuInstruct;
    menuInstruct.setFont(font);
    menuInstruct.setString("Click a mode to begin");
    menuInstruct.setCharacterSize(15);
    menuInstruct.setFillColor(sf::Color(180, 150, 210));
    centerText(menuInstruct, BTN_MENU_X, 478.f);

    // =====================================================================
    // GAME OBJECTS
    // =====================================================================
    Map* map = nullptr;
    Game* game = nullptr;

    sf::RectangleShape panel(sf::Vector2f(panelW, winH));
    panel.setFillColor(sf::Color(14, 8, 28));
    panel.setPosition(mapWidth * tileSize, 0);

    sf::RectangleShape divider(sf::Vector2f(3, winH));
    divider.setFillColor(COL_SUNSET_PINK);
    divider.setPosition(mapWidth * tileSize, 0);

    sf::RectangleShape divider2(sf::Vector2f(1, winH));
    divider2.setFillColor(COL_LAVENDER);
    divider2.setPosition(mapWidth * tileSize + 4, 0);

    float px = mapWidth * tileSize + 12.f;

    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Fatal Exception");
    titleText.setCharacterSize(13);
    titleText.setFillColor(COL_ROSE_GOLD);
    titleText.setPosition(px, 8);

    sf::RectangleShape panelTitleLine(sf::Vector2f(panelW - 16, 1));
    panelTitleLine.setFillColor(COL_DUSK_PURPLE);
    panelTitleLine.setPosition(px, 30);

    sf::Text waveLabel, goldLabel, livesLabel;
    for (auto* t : { &waveLabel, &goldLabel, &livesLabel })
    {
        t->setFont(font);
        t->setCharacterSize(11);
        t->setFillColor(sf::Color(100, 70, 140));
    }
    waveLabel.setString("WAVE");
    goldLabel.setString("TREASURY (PKR)");
    livesLabel.setString("LIVES");
    waveLabel.setPosition(px, 38);
    goldLabel.setPosition(px, 82);
    livesLabel.setPosition(px, 126);

    sf::Text waveVal, goldVal, livesVal;
    waveVal.setFont(font);  waveVal.setCharacterSize(20);  waveVal.setFillColor(COL_PALE_ROSE);
    goldVal.setFont(font);  goldVal.setCharacterSize(20);  goldVal.setFillColor(COL_ROSE_GOLD);
    livesVal.setFont(font); livesVal.setCharacterSize(20); livesVal.setFillColor(COL_SUNSET_PINK);
    waveVal.setPosition(px, 52);
    goldVal.setPosition(px, 96);
    livesVal.setPosition(px, 140);

    sf::RectangleShape sep1(sf::Vector2f(panelW - 16, 2));
    sep1.setFillColor(COL_DEEP_VIOLET);
    sep1.setPosition(px, 170);

    const float SHOP_X = px - 4.f;
    const float SHOP_Y = 176.f;
    const float SHOP_W = panelW - 8.f;
    const float SHOP_H = winH - SHOP_Y - 70.f;

    sf::RectangleShape shopBlock(sf::Vector2f(SHOP_W, SHOP_H));
    shopBlock.setPosition(SHOP_X, SHOP_Y);
    shopBlock.setFillColor(sf::Color(22, 10, 42, 255));
    shopBlock.setOutlineThickness(2.f);
    shopBlock.setOutlineColor(sf::Color(90, 40, 130));

    sf::Text shopHeader;
    shopHeader.setFont(font);
    shopHeader.setString("[ SHOP ]");
    shopHeader.setCharacterSize(14);
    shopHeader.setFillColor(COL_SUNSET_PINK);
    shopHeader.setPosition(SHOP_X + 8.f, SHOP_Y + 6.f);

    sf::RectangleShape shopHeaderLine(sf::Vector2f(SHOP_W - 8.f, 1.f));
    shopHeaderLine.setFillColor(sf::Color(90, 40, 130));
    shopHeaderLine.setPosition(SHOP_X + 4.f, SHOP_Y + 26.f);

    const int   NUM_BTN = 5;
    const float BTN_W = SHOP_W - 16.f;
    const float BTN_H = 40.f;
    const float BTN_X = SHOP_X + 8.f;
    const float BTN_Y0 = SHOP_Y + 32.f;
    const float BTN_GAP = 5.f;

    const string BTN_NAME[NUM_BTN] = { "Cannon","Sniper","MachGun","Slow","Bounce" };
    const int    BTN_COST[NUM_BTN] = { COST_CANNON,COST_SNIPER,COST_MACHGUN,COST_SLOW,COST_BOUNCE };

    const sf::Color BTN_COL[NUM_BTN] = {
        sf::Color(35, 15, 60),
        sf::Color(50, 10, 45),
        sf::Color(25, 10, 55),
        sf::Color(40, 10, 65),
        sf::Color(55, 12, 45)
    };
    const sf::Color BTN_ACCENT[NUM_BTN] = {
        COL_LAVENDER,
        COL_SUNSET_PINK,
        COL_DUSK_PURPLE,
        sf::Color(180, 120, 220),
        COL_ROSE_GOLD
    };

    sf::RectangleShape btnShape[NUM_BTN];
    sf::Text           btnNameText[NUM_BTN];
    sf::Text           btnCostText[NUM_BTN];

    for (int i = 0; i < NUM_BTN; i++)
    {
        float by = BTN_Y0 + i * (BTN_H + BTN_GAP);
        btnShape[i].setSize(sf::Vector2f(BTN_W, BTN_H));
        btnShape[i].setPosition(BTN_X, by);
        btnShape[i].setFillColor(BTN_COL[i]);
        btnShape[i].setOutlineThickness(1.5f);
        btnShape[i].setOutlineColor(sf::Color::Transparent);

        btnNameText[i].setFont(font);
        btnNameText[i].setString(BTN_NAME[i]);
        btnNameText[i].setCharacterSize(14);
        btnNameText[i].setFillColor(BTN_ACCENT[i]);
        btnNameText[i].setPosition(BTN_X + 6.f, by + 4.f);

        btnCostText[i].setFont(font);
        btnCostText[i].setString("PKR " + to_string(BTN_COST[i]));
        btnCostText[i].setCharacterSize(11);
        btnCostText[i].setFillColor(sf::Color(150, 100, 160));
        btnCostText[i].setPosition(BTN_X + 6.f, by + 22.f);
    }

    sf::Text instructText;
    instructText.setFont(font);
    instructText.setCharacterSize(11);
    instructText.setPosition(px, winH - 58.f);

    int selectedTower = 0;

    Enemy* enemies[MAX_ENEMIES] = {};
    bool   goldAwarded[MAX_ENEMIES] = {};

    int   enemyCount = 0;
    int   spawnedCount = 0;
    float spawnTimer = 0.f;
    float spawnDelay = 0.5f;
    bool  waveInProgress = true;

    sf::Vector2f exitPoint, exitPoint2;
    vector<Tower*> towers;

    // ── End-screen ─────
    sf::RectangleShape endOverlay(sf::Vector2f(winW, winH));
    endOverlay.setFillColor(sf::Color(8, 3, 20, 215));

    sf::RectangleShape endTopGlow(sf::Vector2f(winW, 4));
    endTopGlow.setFillColor(COL_SUNSET_PINK);
    sf::RectangleShape endBottomGlow(sf::Vector2f(winW, 4));
    endBottomGlow.setFillColor(COL_SUNSET_PINK);
    endBottomGlow.setPosition(0, winH - 4);

    sf::Text endTitleText, endSubText;
    endTitleText.setFont(font); endTitleText.setCharacterSize(44);
    endSubText.setFont(font);   endSubText.setCharacterSize(20);
    endSubText.setFillColor(COL_LAVENDER);

    sf::RectangleShape endRule(sf::Vector2f(340.f, 2.f));
    endRule.setFillColor(COL_DUSK_PURPLE);
    endRule.setOrigin(170.f, 0);

    sf::RectangleShape restartBtn(sf::Vector2f(240.f, 55.f));
    restartBtn.setOutlineThickness(2.f);
    sf::Text restartBtnText;
    restartBtnText.setFont(font);
    restartBtnText.setString("RESTART");
    restartBtnText.setCharacterSize(22);
    restartBtnText.setFillColor(COL_PALE_ROSE);

    sf::RectangleShape menuBtn(sf::Vector2f(240.f, 55.f));
    menuBtn.setFillColor(COL_DEEP_VIOLET);
    menuBtn.setOutlineThickness(2.f);
    menuBtn.setOutlineColor(COL_LAVENDER);
    sf::Text menuBtnText;
    menuBtnText.setFont(font);
    menuBtnText.setString("MAIN MENU");
    menuBtnText.setCharacterSize(22);
    menuBtnText.setFillColor(COL_LAVENDER);

    const float centerX = (mapWidth * tileSize) / 2.f;
    const float endTitleY = winH / 2.f - 130.f;
    const float endSubY = winH / 2.f - 45.f;
    const float endBtnY = winH / 2.f + 20.f;
    const float endMenuY = winH / 2.f + 90.f;

    sf::Clock clock;

    
    // MAIN LOOP
    
    while (window.isOpen())
    {
        float dt = min(clock.restart().asSeconds(), 0.05f);
        if (ouchCooldown > 0.f) ouchCooldown -= dt;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mpos(
                    static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y));

                // ── MENU ─────────────────────────────────────────────────
                if (gameState == STATE_MENU)
                {
                    auto startGame = [&](int mapIndex)
                        {
                            // Show loading screen while lazy-loading game audio
                            drawLoadingScreen("Loading game audio...");

                            stopMusic(musStart, hasStart);
                            cleanEnemies(enemies, goldAwarded, MAX_ENEMIES);
                            for (Tower* t : towers) delete t;
                            towers.clear();
                            delete map;
                            delete game;

                            // Lazy-load game audio here (not at startup)
                            if (!hasLaser)
                            {
                                hasLaser = musLaser.openFromFile(
                                    "C:/Users/Alizey/source/repos/OOP PROJECT/x64/Debug/Assets/Sounds/Laser.ogg");
                                if (!hasLaser) cerr << "WARNING: Laser.ogg not found\n";
                                else musLaser.setLoop(true);
                            }
                            if (!hasOuch)
                            {
                                hasOuch = musOuch.openFromFile(
                                    "C:/Users/Alizey/source/repos/OOP PROJECT/x64/Debug/Assets/Sounds/Ouch.ogg");
                                if (!hasOuch) cerr << "WARNING: Ouch.ogg not found\n";
                            }
                            // Win/Lose loaded lazily when game ends (see below)

                            selectedMap = mapIndex;
                            map = new Map(mapWidth, mapHeight, tileSize, mapIndex);
                            game = new Game();

                            const auto& path1 = map->getPath();
                            exitPoint = path1.empty() ? sf::Vector2f(0, 0) : path1.back();

                            if (mapIndex == 1 || mapIndex == 2)
                            {
                                const auto& path2 = map->getPath2();
                                exitPoint2 = path2.empty() ? exitPoint : path2.back();
                            }
                            else
                                exitPoint2 = exitPoint;

                            enemyCount = game->getEnemyCountForWave();
                            spawnedCount = 0;
                            spawnTimer = 0.f;
                            waveInProgress = true;
                            gameState = STATE_PLAYING;
                            soundEndPlayed = false;
                            ouchCooldown = 0.f;

                            stopMusic(musLose, hasLose);
                            stopMusic(musWin, hasWin);
                            playMusic(musLaser, hasLaser);
                        };

                    if (btnBeginner.getGlobalBounds().contains(mpos)) startGame(0);
                    else if (btnPro.getGlobalBounds().contains(mpos))      startGame(1);
                    else if (btnChad.getGlobalBounds().contains(mpos))     startGame(2);
                    continue;
                }

                // ── END SCREEN ───────────────────────────────────────────
                if (game && (game->isGameOver() || game->isWon()))
                {
                    if (restartBtn.getGlobalBounds().contains(mpos))
                    {
                        resetGame(*game, enemies, goldAwarded, MAX_ENEMIES,
                            spawnedCount, enemyCount, spawnTimer, waveInProgress, towers);
                        stopMusic(musLose, hasLose);
                        stopMusic(musWin, hasWin);
                        soundEndPlayed = false;
                        playMusic(musLaser, hasLaser);
                    }
                    else if (menuBtn.getGlobalBounds().contains(mpos))
                    {
                        resetGame(*game, enemies, goldAwarded, MAX_ENEMIES,
                            spawnedCount, enemyCount, spawnTimer, waveInProgress, towers);
                        stopMusic(musLaser, hasLaser);
                        stopMusic(musLose, hasLose);
                        stopMusic(musWin, hasWin);
                        stopMusic(musStart, hasStart);
                        soundEndPlayed = false;
                        gameState = STATE_MENU;
                    }
                    continue;
                }

                // ── TOWER PANEL ──────────────────────────────────────────
                bool hitBtn = false;
                for (int i = 0; i < NUM_BTN; i++)
                {
                    if (btnShape[i].getGlobalBounds().contains(mpos))
                    {
                        selectedTower = i;
                        hitBtn = true;
                        break;
                    }
                }

                if (!hitBtn && mpos.x < mapWidth * tileSize && map && game)
                {
                    int tx = static_cast<int>(mpos.x) / tileSize;
                    int ty = static_cast<int>(mpos.y) / tileSize;

                    if (tx >= 0 && tx < mapWidth && ty >= 0 && ty < mapHeight)
                    {
                        if (map->getGridValue(tx, ty) == 0)
                        {
                            if (game->spendGold(getCost(selectedTower)))
                            {
                                sf::Vector2f tpos(
                                    tx * tileSize + tileSize / 2.f,
                                    ty * tileSize + tileSize / 2.f);
                                towers.push_back(buildTower(selectedTower, tpos));
                            }
                        }
                    }
                }
            }
        }

        // ── GAME LOGIC 
        if (gameState == STATE_PLAYING && map && game)
        {
            if (!soundEndPlayed)
            {
                if (game->isWon())
                {
                    stopMusic(musLaser, hasLaser);

                    // Lazy-load win music the first time it's needed
                    if (!hasWin)
                    {
                        hasWin = musWin.openFromFile(
                            "C:/Users/Alizey/source/repos/OOP PROJECT/x64/Debug/Assets/Sounds/Win.ogg");
                        if (!hasWin) cerr << "WARNING: Win.ogg not found\n";
                    }
                    playMusic(musWin, hasWin);
                    soundEndPlayed = true;
                }
                else if (game->isGameOver())
                {
                    stopMusic(musLaser, hasLaser);

                    // Lazy-load lose music the first time it's needed
                    if (!hasLose)
                    {
                        hasLose = musLose.openFromFile(
                            "C:/Users/Alizey/source/repos/OOP PROJECT/x64/Debug/Assets/Sounds/Lose.ogg");
                        if (!hasLose) cerr << "WARNING: Lose.ogg not found\n";
                    }
                    playMusic(musLose, hasLose);
                    soundEndPlayed = true;
                }
            }

            if (!game->isGameOver() && !game->isWon())
            {
                // Spawn
                if (waveInProgress &&
                    spawnedCount < enemyCount &&
                    spawnedCount < MAX_ENEMIES)
                {
                    spawnTimer += dt;
                    if (spawnTimer >= spawnDelay)
                    {
                        spawnTimer = 0.f;

                        bool useBottomPath =
                            ((selectedMap == 1 || selectedMap == 2) && spawnedCount % 2 == 1);
                        sf::Vector2f useExit = useBottomPath ? exitPoint2 : exitPoint;

                        Enemy* e = spawnEnemy(game->getWave(), spawnedCount, useExit);
                        if (e)
                        {
                            const auto& chosenPath =
                                useBottomPath ? map->getPath2() : map->getPath();

                            if (!chosenPath.empty())
                            {
                                e->setPath(chosenPath);
                                e->setStartPosition(chosenPath[0]);
                                enemies[spawnedCount] = e;
                                goldAwarded[spawnedCount] = false;
                                spawnedCount++;
                            }
                            else { delete e; }
                        }
                    }
                }

                int aliveCount = 0;
                vector<Enemy*> liveEnemies;

                for (int i = 0; i < spawnedCount; i++)
                {
                    if (!enemies[i]) continue;

                    if (!enemies[i]->isDead())
                    {
                        enemies[i]->update(dt);

                        if (enemies[i]->hasReachedEnd())
                        {
                            *game -= 1;

                            if (hasOuch && ouchCooldown <= 0.f)
                            {
                                musOuch.play();
                                ouchCooldown = OUCH_MIN_INTERVAL;
                            }

                            delete enemies[i];
                            enemies[i] = nullptr;
                        }
                        else
                        {
                            aliveCount++;
                            liveEnemies.push_back(enemies[i]);
                        }
                    }
                    else
                    {
                        if (!goldAwarded[i] && enemies[i]->getHP() == 0)
                        {
                            *game += 100;
                            goldAwarded[i] = true;
                        }
                    }
                }

                if (waveInProgress && spawnedCount == enemyCount && aliveCount == 0)
                    waveInProgress = false;

                if (!waveInProgress && !game->isWon())
                {
                    ++(*game);
                    if (!game->isWon())
                    {
                        cleanEnemies(enemies, goldAwarded, MAX_ENEMIES);
                        enemyCount = game->getEnemyCountForWave();
                        spawnedCount = 0;
                        spawnTimer = 0.f;
                        waveInProgress = true;
                    }
                }

                for (Tower* t : towers)
                {
                    t->update(dt);
                    t->attack(liveEnemies);
                }
            }

            // HUD values
            waveVal.setString(to_string(game->getWave()) + " / " +
                to_string(game->getMaxWaves()));
            goldVal.setString("Rs. " + to_string(game->getGold()));
            livesVal.setString(to_string(game->getLives()));

            bool canAfford = game->getGold() >= getCost(selectedTower);
            string instrStr = "Placing: " + BTN_NAME[selectedTower] + "\n" +
                "Cost: PKR " + to_string(getCost(selectedTower));
            if (!canAfford) instrStr += "\n[NOT ENOUGH PKR!]";
            instructText.setString(instrStr);
            instructText.setFillColor(canAfford ? COL_LAVENDER : sf::Color(230, 60, 100));

            for (int i = 0; i < NUM_BTN; i++)
                btnShape[i].setOutlineColor(
                    i == selectedTower ? BTN_ACCENT[i] : sf::Color::Transparent);

            // ── End-screen layout 
            if (game->isWon() || game->isGameOver())
            {
                if (game->isWon())
                {
                    endTitleText.setString("YOU WIN!! Time to Touch Grass Now");
                    endTitleText.setFillColor(COL_ROSE_GOLD);
                    endSubText.setString("All waves cleared! Great defense!");
                    restartBtn.setFillColor(sf::Color(30, 12, 55));
                    restartBtn.setOutlineColor(COL_LAVENDER);
                }
                else
                {
                    endTitleText.setString("YOU LOSE!! WOMP WOMP");
                    endTitleText.setFillColor(COL_SUNSET_PINK);
                    endSubText.setString("No lives remaining. Try again!");
                    restartBtn.setFillColor(sf::Color(50, 8, 35));
                    restartBtn.setOutlineColor(COL_SUNSET_PINK);
                }

                {
                    sf::FloatRect b = endTitleText.getLocalBounds();
                    endTitleText.setOrigin(b.left + b.width / 2.f, b.top);
                    endTitleText.setPosition(centerX, endTitleY);
                }
                endRule.setPosition(centerX, endTitleY + 62.f);
                {
                    sf::FloatRect b = endSubText.getLocalBounds();
                    endSubText.setOrigin(b.left + b.width / 2.f, b.top);
                    endSubText.setPosition(centerX, endSubY);
                }

                restartBtn.setOrigin(restartBtn.getSize().x / 2.f, 0);
                restartBtn.setPosition(centerX, endBtnY);
                {
                    sf::FloatRect b = restartBtnText.getLocalBounds();
                    restartBtnText.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
                    restartBtnText.setPosition(centerX, endBtnY + 27.f);
                }

                menuBtn.setOrigin(menuBtn.getSize().x / 2.f, 0);
                menuBtn.setPosition(centerX, endMenuY);
                {
                    sf::FloatRect b = menuBtnText.getLocalBounds();
                    menuBtnText.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
                    menuBtnText.setPosition(centerX, endMenuY + 27.f);
                }
            }
        }

        // ── DRAW 
        window.clear(COL_VOID);

        if (gameState == STATE_MENU)
        {
            window.draw(menuSprite);
            window.draw(darkOverlay);
            window.draw(menuTitle);
            window.draw(titleUnderline);
            window.draw(menuSub);
            window.draw(btnBeginner);
            window.draw(btnBeginnerTitle);
            window.draw(btnPro);
            window.draw(btnProTitle);
            window.draw(btnChad);
            window.draw(btnChadTitle);
            window.draw(menuInstruct);
        }

        if (gameState == STATE_PLAYING && map && game)
        {
            map->draw(window);

            for (int i = 0; i < spawnedCount; i++)
            {
                if (!enemies[i] || enemies[i]->isDead()) continue;

                sf::Vector2f pos = enemies[i]->getPosition();
                int          tidx = enemyTexIndex(enemies[i]);

                if (enemyTexOk[tidx])
                {
                    const float SZ = 32.f;
                    sf::Sprite spr(enemyTex[tidx]);
                    sf::Vector2u ts = enemyTex[tidx].getSize();
                    spr.setScale(SZ / ts.x, SZ / ts.y);
                    spr.setOrigin(ts.x / 2.f, ts.y / 2.f);
                    spr.setPosition(pos.x + 20.f, pos.y + 20.f);
                    window.draw(spr);
                }
                else
                {
                    enemies[i]->render(window);
                }

                float ratio = static_cast<float>(enemies[i]->getHP()) /
                    static_cast<float>(enemies[i]->getMaxHP());
                ratio = max(0.f, min(1.f, ratio));

                sf::RectangleShape back(sf::Vector2f(36, 5));
                back.setFillColor(sf::Color(60, 10, 10));
                back.setPosition(pos.x + 2.f, pos.y - 4.f);

                sf::RectangleShape front(sf::Vector2f(36.f * ratio, 5));
                front.setFillColor(sf::Color(220, 30, 30));
                front.setPosition(pos.x + 2.f, pos.y - 4.f);

                window.draw(back);
                window.draw(front);
            }

            for (int i = 0; i < (int)towers.size(); i++)
            {
                int          ttype = towerTexIndex(towers[i]);
                sf::Vector2f tpos = towers[i]->getPosition();

                if (towerTexOk[ttype])
                {
                    const float SZ = 38.f;
                    sf::Sprite spr(towerTex[ttype]);
                    sf::Vector2u ts = towerTex[ttype].getSize();
                    spr.setScale(SZ / ts.x, SZ / ts.y);
                    spr.setOrigin(ts.x / 2.f, ts.y / 2.f);
                    spr.setPosition(tpos);
                    window.draw(spr);
                }
                else
                {
                    towers[i]->render(window);
                    drawTowerShape(window, tpos, ttype, BTN_ACCENT[ttype]);
                }
            }

            if (game->isWon() || game->isGameOver())
            {
                window.draw(endOverlay);
                window.draw(endTopGlow);
                window.draw(endBottomGlow);
                window.draw(endTitleText);
                window.draw(endRule);
                window.draw(endSubText);
                window.draw(restartBtn);
                window.draw(restartBtnText);
                window.draw(menuBtn);
                window.draw(menuBtnText);
            }

            window.draw(panel);
            window.draw(divider);
            window.draw(divider2);
            window.draw(panelTitleLine);
            window.draw(titleText);
            window.draw(waveLabel);  window.draw(waveVal);
            window.draw(goldLabel);  window.draw(goldVal);
            window.draw(livesLabel); window.draw(livesVal);
            window.draw(sep1);
            window.draw(shopBlock);
            window.draw(shopHeader);
            window.draw(shopHeaderLine);

            for (int i = 0; i < NUM_BTN; i++)
            {
                window.draw(btnShape[i]);
                window.draw(btnNameText[i]);
                window.draw(btnCostText[i]);
            }

            window.draw(instructText);
        }

        window.display();
    }

    // ── CLEANUP ───────────────────────────────────────────────────────────
    stopMusic(musLaser, hasLaser);
    stopMusic(musStart, hasStart);
    stopMusic(musLose, hasLose);
    stopMusic(musWin, hasWin);

    cleanEnemies(enemies, goldAwarded, MAX_ENEMIES);
    for (Tower* t : towers) delete t;
    delete map;
    delete game;

    return 0;
}