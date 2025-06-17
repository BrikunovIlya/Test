#pragma once

constexpr float GRAVITY = 0.5f;
constexpr float JUMP_FORCE = -8.0f;
constexpr float MOVE_ACCEL = 0.8f;
constexpr float MAX_SPEED = 4.0f;
constexpr float FRICTION = 0.85f;

namespace Physics {
    void ApplyGravity(float& dy, bool onGround);
    void ApplyFriction(float& dx);
    void LimitVelocity(float& dx, float& dy);
    bool CheckCollision(float x, float y, const std::vector<std::vector<int>>& map);
}