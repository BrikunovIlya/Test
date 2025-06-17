#include "physics.h"

void Physics::ApplyGravity(float& dy, bool onGround) {
    if (!onGround) dy += GRAVITY;
}

void Physics::ApplyFriction(float& dx) {
    dx *= FRICTION;
    if (abs(dx) < 0.1f) dx = 0;
}

void Physics::LimitVelocity(float& dx, float& dy) {
    dx = std::max(-MAX_SPEED, std::min(MAX_SPEED, dx));
    dy = std::max(-MAX_SPEED * 2, std::min(MAX_SPEED * 2, dy));
}

bool Physics::CheckCollision(float x, float y, const std::vector<std::vector<int>>& map) {
    int ix = static_cast<int>(x);
    int iy = static_cast<int>(y);
    
    if (ix < 0 || ix >= map[0].size() || iy < 0 || iy >= map.size()) 
        return true;
    
    return map[iy][ix] == 1; // 1 = Wall
}