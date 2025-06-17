#include "enemy.h"
#include "../utils/console.h"

Enemy::Enemy(float startX, float startY, Type enemyType) 
    : x(startX), y(startY), type(enemyType) {
    health = (enemyType == BOSS) ? 200 : 50;
}

void Enemy::Update(std::vector<std::vector<int>>& map) {
    // Простейший ИИ
    dx = (x < 30) ? 0.5f : -0.5f;
    
    Physics::ApplyGravity(dy, onGround);
    Physics::LimitVelocity(dx, dy);
    
    float newX = x + dx;
    if (!Physics::CheckCollision(newX, y, map)) {
        x = newX;
    }
    
    float newY = y + dy;
    if (!Physics::CheckCollision(x, newY, map)) {
        y = newY;
        onGround = false;
    } else {
        if (dy > 0) onGround = true;
        dy = 0;
    }
}

void Enemy::Draw(int offsetX, int offsetY) const {
    int screenX = static_cast<int>(x) - offsetX;
    int screenY = static_cast<int>(y) - offsetY;
    
    Console::SetCursorPosition(screenX, screenY);
    Console::SetColor((type == BOSS) ? BRIGHT_MAGENTA : BRIGHT_RED);
    std::cout << 'E';
}