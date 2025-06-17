#include "player.h"
#include "../utils/console.h"
#include <algorithm>
#include <iostream>

Player::Player(float startX, float startY) : x(startX), y(startY) {
    abilities["double_jump"] = false;
    abilities["dash"] = false;
    abilities["wall_jump"] = false;
    checkpointX = static_cast<int>(x);
    checkpointY = static_cast<int>(y);
}

void Player::Update(std::vector<std::vector<int>>& map) {
    // Apply physics
    Physics::ApplyGravity(dy, onGround);
    Physics::LimitVelocity(dx, dy);
    
    // Movement
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
    
    // Update cooldowns
    if (dashCooldown > 0) dashCooldown--;
    if (dashTimer > 0) dashTimer--;
    if (attackCooldown > 0) attackCooldown--;
    if (invincibility > 0) invincibility--;
    
    UpdateProjectiles();
}

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

void Player::Dash() {
    if (dashCooldown == 0 && abilities["dash"]) {
        dx = facingRight ? MAX_SPEED * 3 : -MAX_SPEED * 3;
        dashTimer = 10;
        dashCooldown = 60;
        dy = 0;
    }
}

void Player::Attack() {
    if (attackCooldown == 0) {
        attackCooldown = 20;
        projectiles.push_back({x + (facingRight ? 1 : -1), y});
    }
}

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

void Player::Draw(int offsetX, int offsetY) const {
    if (invincibility > 0 && invincibility % 4 < 2) return;
    
    int screenX = static_cast<int>(x) - offsetX;
    int screenY = static_cast<int>(y) - offsetY;
    
    Console::SetCursorPosition(screenX, screenY);
    Console::SetColor(BRIGHT_CYAN);
    std::cout << '@';
}