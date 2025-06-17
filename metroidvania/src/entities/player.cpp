#include "player.h"
#include "../utils/console.h"
#include <algorithm>
#include <iostream>

/**
 * @brief Конструктор класса Player, задающий начальную позицию персонажа.
 * @param startX Начальная координата x персонажа.
 * @param startY Начальная координата y персонажа.
 */
Player::Player(float startX, float startY) : x(startX), y(startY) {
    abilities["double_jump"] = false;
    abilities["dash"] = false;
    abilities["wall_jump"] = false;
    checkpointX = static_cast<int>(x);
    checkpointY = static_cast<int>(y);
}

/**
 * @brief Обновляет состояние персонажа, включая движение, физику и перезарядку.
 * @param map Игровая карта для проверки столкновений.
 */
void Player::Update(std::vector<std::vector<int>>& map) {
    // Применение физики
    Physics::ApplyGravity(dy, onGround);
    Physics::LimitVelocity(dx, dy);
    
    // Движение
    float newX = x + dx;
    if (!Physics::CheckCollision(newX, y, map)) {
        x = newX;
    } else {
        dx = 0;
    }
    
    float newY = y + dy;
    if (!Physics::CheckCollision(x, newY, map)) {
        y = newY;
        onGround = false;
    } else {
        if (dy > 0) onGround = true;
        dy = 0;
    }
    
    Physics::ApplyFriction(dx);
    
    // Обновление таймеров перезарядки
    if (dashCooldown > 0) dashCooldown--;
    if (dashTimer > 0) dashTimer--;
    if (attackCooldown > 0) attackCooldown--;
    if (invincibility > 0) invincibility--;
    
    UpdateProjectiles();
}

/**
 * @brief Выполняет прыжок или двойной прыжок, если доступно.
 */
void Player::Jump() {
    if (onGround) {
        dy = JUMP_FORCE;
        onGround = false;
        hasDoubleJump = true;
    } else if (hasDoubleJump && abilities["double_jump"]) {
        dy = JUMP_FORCE * 0.8f;
        hasDoubleJump = false;
    }
}

/**
 * @brief Инициирует рывок, если способность доступна и не на перезарядке.
 */
void Player::Dash() {
    if (dashCooldown == 0 && abilities["dash"]) {
        dx = facingRight ? MAX_SPEED * 3 : -MAX_SPEED * 3;
        dashTimer = 10;
        dashCooldown = 60;
        dy = 0;
    }
}

/**
 * @brief Выполняет атаку, создавая снаряд, если атака не на перезарядке.
 */
void Player::Attack() {
    if (attackCooldown == 0) {
        attackCooldown = 20;
        projectiles.push_back({x + (facingRight ? 1 : -1), y});
    }
}

/**
 * @brief Обновляет позиции всех активных снарядов и удаляет те, что вышли за границы.
 */
void Player::UpdateProjectiles() {
    for (auto it = projectiles.begin(); it != projectiles.end(); ) {
        it->first += facingRight ? 0.8f : -0.8f;
        if (it->first < 0 || it->first > 100) {
            it = projectiles.erase(it);
        } else {
            ++it;
        }
    }
}

/**
 * @brief Отрисовывает персонажа на экране с учетом смещения.
 * @param offsetX Смещение по x для рендеринга (позиция камеры).
 * @param offsetY Смещение по y для рендеринга (позиция камеры).
 */
void Player::Draw(int offsetX, int offsetY) const {
    if (invincibility > 0 && invincibility % 4 < 2) return;
    
    int screenX = static_cast<int>(x) - offsetX;
    int screenY = static_cast<int>(y) - offsetY;
    
    Console::SetCursorPosition(screenX, screenY);
    Console::SetColor(BRIGHT_CYAN);
    std::cout << '@';
}