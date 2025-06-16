#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>  // Add this include for remove_if

using namespace std;

const int MAP_WIDTH = 40;
const int MAP_HEIGHT = 20;
const int GRAVITY = 1;

class Player {
public:
    int x, y;
    int health;
    int maxHealth;
    bool onGround;
    int jumpPower;
    bool facingRight;
    
    Player(int startX, int startY) : x(startX), y(startY), health(100), maxHealth(100), 
                                   onGround(false), jumpPower(0), facingRight(true) {}
    
    void jump() {
        if (onGround) {
            jumpPower = 3;
            onGround = false;
        }
    }
    
    void moveLeft() {
        facingRight = false;
        if (x > 0) x--;
    }
    
    void moveRight() {
        facingRight = true;
        if (x < MAP_WIDTH - 1) x++;
    }
    
    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }
    
    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
    }
    
    bool isAlive() const {
        return health > 0;
    }
};

class Enemy {
public:
    int x, y;
    int health;
    bool alive;
    int moveDirection;
    
    Enemy(int startX, int startY) : x(startX), y(startY), health(30), alive(true), moveDirection(1) {}
    
    void update() {
        if (!alive) return;
        
        // Simple AI: move back and forth
        x += moveDirection;
        if (x <= 1 || x >= MAP_WIDTH - 2) {
            moveDirection *= -1;
        }
    }
    
    void takeDamage(int damage) {
        health -= damage;
        if (health <= 0) {
            alive = false;
        }
    }
};

class Projectile {
public:
    int x, y;
    int direction; // 1 for right, -1 for left
    bool active;
    
    Projectile(int startX, int startY, int dir) : x(startX), y(startY), direction(dir), active(true) {}
    
    void update() {
        if (!active) return;
        x += direction * 2;
        if (x < 0 || x >= MAP_WIDTH) {
            active = false;
        }
    }
};

class Game {
private:
    vector<vector<char>> map;
    Player player;
    vector<Enemy> enemies;
    vector<Projectile> projectiles;
    int score;
    
public:
    Game() : player(5, 10), score(0) {
        initializeMap();
        spawnEnemies();
    }
    
    void initializeMap() {
        map.resize(MAP_HEIGHT, vector<char>(MAP_WIDTH, ' '));
        
        // Create ground level
        for (int x = 0; x < MAP_WIDTH; x++) {
            map[MAP_HEIGHT - 1][x] = '#';
            map[MAP_HEIGHT - 2][x] = '#';
        }
        
        // Create platforms
        for (int x = 10; x < 20; x++) {
            map[15][x] = '#';
        }
        
        for (int x = 25; x < 35; x++) {
            map[12][x] = '#';
        }
        
        for (int x = 5; x < 15; x++) {
            map[8][x] = '#';
        }
        
        // Create walls
        for (int y = 0; y < MAP_HEIGHT; y++) {
            map[y][0] = '#';
            map[y][MAP_WIDTH - 1] = '#';
        }
        
        // Create ceiling
        for (int x = 0; x < MAP_WIDTH; x++) {
            map[0][x] = '#';
        }
        
        // Add some decorative elements
        map[14][12] = 'H'; // Health pickup
        map[11][30] = 'H'; // Health pickup
        map[7][10] = '*';  // Collectible
    }
    
    void spawnEnemies() {
        enemies.push_back(Enemy(15, 14));
        enemies.push_back(Enemy(30, 11));
        enemies.push_back(Enemy(20, MAP_HEIGHT - 3));
    }
    
    void clearScreen() {
        system("cls");
    }
    
    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }
    
    void render() {
        clearScreen();
        
        // Create display map
        vector<vector<char>> displayMap = map;
        
        // Add player to display
        if (player.isAlive()) {
            displayMap[player.y][player.x] = player.facingRight ? '>' : '<';
        }
        
        // Add enemies to display
        for (const auto& enemy : enemies) {
            if (enemy.alive) {
                displayMap[enemy.y][enemy.x] = 'E';
            }
        }
        
        // Add projectiles to display
        for (const auto& projectile : projectiles) {
            if (projectile.active) {
                displayMap[projectile.y][projectile.x] = '-';
            }
        }
        
        // Render the map
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                cout << displayMap[y][x];
            }
            cout << endl;
        }
        
        // Display UI
        cout << "\n===========================================\n";
        cout << "Health: " << player.health << "/" << player.maxHealth;
        cout << " | Score: " << score;
        cout << " | Enemies: " << countAliveEnemies();
        cout << "\n===========================================\n";
        cout << "Controls: A/D - Move, W - Jump, S - Shoot, Q - Quit\n";
    }
    
    int countAliveEnemies() {
        int count = 0;
        for (const auto& enemy : enemies) {
            if (enemy.alive) count++;
        }
        return count;
    }
    
    bool isCollision(int x, int y) {
        if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return true;
        return map[y][x] == '#';
    }
    
    void updatePhysics() {
        if (!player.isAlive()) return;
        
        // Apply gravity and jumping
        if (player.jumpPower > 0) {
            if (!isCollision(player.x, player.y - 1)) {
                player.y--;
                player.jumpPower--;
            } else {
                player.jumpPower = 0;
            }
        } else {
            // Apply gravity
            if (!isCollision(player.x, player.y + 1)) {
                player.y++;
                player.onGround = false;
            } else {
                player.onGround = true;
            }
        }
        
        // Check for ground collision
        if (isCollision(player.x, player.y)) {
            player.y--;
            player.onGround = true;
        }
    }
    
    void updateEnemies() {
        for (auto& enemy : enemies) {
            if (enemy.alive) {
                enemy.update();
                
                // Check collision with ground
                if (!isCollision(enemy.x, enemy.y + 1)) {
                    enemy.y++;
                }
                
                // Check collision with player
                if (enemy.x == player.x && enemy.y == player.y) {
                    player.takeDamage(10);
                }
            }
        }
    }
    
    void updateProjectiles() {
        for (auto& projectile : projectiles) {
            if (projectile.active) {
                projectile.update();
                
                // Check collision with walls
                if (isCollision(projectile.x, projectile.y)) {
                    projectile.active = false;
                }
                
                // Check collision with enemies
                for (auto& enemy : enemies) {
                    if (enemy.alive && enemy.x == projectile.x && enemy.y == projectile.y) {
                        enemy.takeDamage(20);
                        projectile.active = false;
                        if (!enemy.alive) {
                            score += 100;
                        }
                        break;
                    }
                }
            }
        }
        
        // Remove inactive projectiles - FIXED VERSIO
        projectiles.erase(
            std::remove_if(projectiles.begin(), projectiles.end(),
                          [](const Projectile& p) { return !p.active; }),
            projectiles.end()
        );
    }
    
    void checkPickups() {
        char& currentTile = map[player.y][player.x];
        
        if (currentTile == 'H') {
            player.heal(25);
            currentTile = ' ';
            score += 50;
        } else if (currentTile == '*') {
            currentTile = ' ';
            score += 200;
        }
    }
    
    void handleInput() {
        if (_kbhit()) {
            char key = _getch();
            key = tolower(key);
            
            switch (key) {
                case 'a':
                    if (!isCollision(player.x - 1, player.y)) {
                        player.moveLeft();
                    }
                    break;
                case 'd':
                    if (!isCollision(player.x + 1, player.y)) {
                        player.moveRight();
                    }
                    break;
                case 'w':
                    player.jump();
                    break;
                case 's':
                    // Shoot projectile
                    projectiles.push_back(Projectile(
                        player.x + (player.facingRight ? 1 : -1),
                        player.y,
                        player.facingRight ? 1 : -1
                    ));
                    break;
                case 'q':
                    exit(0);
                    break;
            }
        }
    }
    
    bool isGameWon() {
        return countAliveEnemies() == 0;
    }
    
    void run() {
        while (player.isAlive()) {
            handleInput();
            updatePhysics();
            updateEnemies();
            updateProjectiles();
            checkPickups();
            render();
            
            if (isGameWon()) {
                cout << "\n*** VICTORY! All enemies defeated! ***\n";
                cout << "Final Score: " << score << endl;
                cout << "Press any key to exit...";
                _getch();
                break;
            }
            
            Sleep(100); // Small delay for smoother gameplay
        }
        
        if (!player.isAlive()) {
            cout << "\n*** GAME OVER ***\n";
            cout << "Final Score: " << score << endl;
            cout << "Press any key to exit...";
            _getch();
        }
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));
    
    cout << "=== METROID-LIKE GAME ===\n";
    cout << "Defeat all enemies to win!\n";
    cout << "Collect H for health and * for points\n";
    cout << "Press any key to start...";
    _getch();
    
    Game game;
    game.run();
    
    return 0;
}