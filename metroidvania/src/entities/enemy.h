#pragma once
#include "../systems/physics.h"
#include <vector>

/**
 * @brief Класс, представляющий врага в игре.
 * 
 * Класс Enemy управляет позицией, движением и типом врага (Дрон или Босс),
 * включая базовое поведение ИИ и отрисовку.
 */
class Enemy {
public:
    /** @brief Перечисление, определяющее типы врагов. */
    enum Type { DRONE, BOSS };
    
    /** @brief Координата x врага в игровом мире. */
    float x;
    /** @brief Координата y врага в игровом мире. */
    float y;
    /** @brief Горизонтальная скорость врага. */
    float dx = 0;
    /** @brief Вертикальная скорость врага. */
    float dy = 0;
    /** @brief Тип врага (DRONE или BOSS). */
    Type type;
    /** @brief Текущее здоровье врага. */
    int health;
    /** @brief Указывает, жив ли враг. */
    bool alive = true;
    /** @brief Указывает, находится ли враг на земле. */
    bool onGround = false;
    
    /**
     * @brief Конструктор класса Enemy, задающий начальную позицию и тип врага.
     * @param startX Начальная координата x врага.
     * @param startY Начальная координата y врага.
     * @param enemyType Тип врага (DRONE или BOSS).
     */
    Enemy(float startX, float startY, Type enemyType);

    /**
     * @brief Обновляет состояние врага, включая движение и поведение ИИ.
     * @param map Игровая карта для проверки столкновений.
     */
    void Update(std::vector<std::vector<int>>& map);

    /**
     * @brief Отрисовывает врага на экране с учетом смещения.
     * @param offsetX Смещение по x для рендеринга (позиция камеры).
     * @param offsetY Смещение по y для рендеринга (позиция камеры).
     */
    void Draw(int offsetX, int offsetY) const;
};