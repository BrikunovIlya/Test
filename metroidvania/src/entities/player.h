#pragma once
#include "physics.h"
#include <vector>
#include <map>

class Player {
public:
    float x, y;
    float dx = 0, dy = 0;
    int health = 100;
    int maxHealth = 100;
    bool onGround = false;
    bool facingRight = true;
    int invincibility = 0;
    int keys = 0;
    int coins = 0;
    bool hasDoubleJump = false;
    int dashCooldown = 0;
    int dashTimer = 0;
    int attackCooldown = 0;
    std::vector<std::pair<float, float>> projectiles;
    std::map<std::string, bool> abilities;
    int checkpointX = 0, checkpointY = 0;
    int score = 0;

    Player(float startX, float startY);
    void Update(std::vector<std::vector<int>>& map);
    void Jump();
    void Dash();
    void Attack();
    void UpdateProjectiles();
    void Draw(int offsetX, int offsetY) const;
};