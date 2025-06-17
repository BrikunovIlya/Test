#include "physics.h"

/**
 * @brief Применяет гравитацию к вертикальной скорости объекта.
 * @param dy Вертикальная скорость объекта (модифицируется).
 * @param onGround Указывает, находится ли объект на земле.
 */
void Physics::ApplyGravity(float& dy, bool onGround) {
    if (!onGround) dy += GRAVITY;
}

/**
 * @brief Применяет трение к горизонтальной скорости объекта.
 * @param dx Горизонтальная скорость объекта (модифицируется).
 */
void Physics::ApplyFriction(float& dx) {
    dx *= FRICTION;
    if (abs(dx) < 0.1f) dx = 0;
}

/**
 * @brief Ограничивает горизонтальную и вертикальную скорость объекта.
 * @param dx Горизонтальная скорость объекта (модифицируется).
 * @param dy Вертикальная скорость объекта (модифицируется).
 */
void Physics::LimitVelocity(float& dx, float& dy) {
    dx = std::max(-MAX_SPEED, std::min(MAX_SPEED, dx));
    dy = std::max(-MAX_SPEED * 2, std::min(MAX_SPEED * 2, dy));
}

/**
 * @brief Проверяет наличие столкновения с тайлом карты.
 * @param x Координата x для проверки.
 * @param y Координата y для проверки.
 * @param map Игровая карта для проверки столкновений.
 * @return true, если есть столкновение (тайл является стеной или координаты вне карты), иначе false.
 */
bool Physics::CheckCollision(float x, float y, const std::vector<std::vector<int>>& map) {
    int ix = static_cast<int>(x);
    int iy = static_cast<int>(y);
    
    if (ix < 0 || ix >= map[0].size() || iy < 0 || iy >= map.size()) 
        return true;
    
    return map[iy][ix] == 1; // 1 = Wall
}