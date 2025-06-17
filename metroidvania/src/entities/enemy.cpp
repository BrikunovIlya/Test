#include "enemy.h"
#include "../utils/console.h"

/**
 * @brief Конструктор класса Enemy, задающий начальную позицию и тип врага.
 * @param startX Начальная координата x врага.
 * @param startY Начальная координата y врага.
 * @param enemyType Тип врага (DRONE или BOSS).
 */
Enemy::Enemy(float startX, float startY, Type enemyType) 
    : x(startX), y(startY), type(enemyType) {
    health = (enemyType == BOSS) ? 200 : 50;
}

/**
 * @brief Обновляет состояние врага, включая движение и поведение ИИ.
 * @param map Игровая карта для проверки столкновений.
 */
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

/**
 * @brief Отрисовывает врага на экране с учетом смещения.
 * @param offsetX Смещение по x для рендеринга (позиция камеры).
 * @param offsetY Смещение по y для рендеринга (позиция камеры).
 */
void Enemy::Draw(int offsetX, int offsetY) const {
    int screenX = static_cast<int>(x) - offsetX;
    int screenY = static_cast<int>(y) - offsetY;
    
    Console::SetCursorPosition(screenX, screenY);
    Console::SetColor((type == BOSS) ? BRIGHT_MAGENTA : BRIGHT_RED);
    std::cout << 'E';
}