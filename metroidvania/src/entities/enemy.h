#pragma once
#include "../systems/physics.h"
#include <vector>

class Enemy {
public:
    enum Type { DRONE, BOSS };
    
    float x, y;
    float dx = 0, dy = 0;
    Type type;
    int health;
    bool alive = true;
    bool onGround = false;
    
    Enemy(float startX, float startY, Type enemyType);
    void Update(std::vector<std::vector<int>>& map);
    void Draw(int offsetX, int offsetY) const;
};