#include "level.h"

/**
 * @brief Конструктор класса Level.
 * 
 * Инициализирует карту уровня с заданными размерами и вызывает генерацию тестового уровня.
 */
Level::Level() {
    map.resize(height, std::vector<int>(width, 0));
    GenerateTestLevel();
}

/**
 * @brief Генерирует тестовый уровень.
 * 
 * Создает уровень с границами, платформами, шипами и предметами (дверь, ключ, способности, здоровье, контрольная точка).
 */
void Level::GenerateTestLevel() {
    // Границы
    for (int y = 0; y < height; y++) {
        map[y][0] = 1;
        map[y][width - 1] = 1;
    }
    for (int x = 0; x < width; x++) {
        map[0][x] = 1;
        map[height - 1][x] = 1;
    }
    
    // Платформы
    for (int x = 5; x < 15; x++) map[15][x] = 1;
    for (int x = 25; x < 35; x++) map[15][x] = 1;
    for (int x = 10; x < 30; x++) map[10][x] = 1;
    
    // Шипы
    for (int x = 17; x < 23; x++) map[16][x] = 2;
    
    // Предметы
    map[9][19] = 3; // Дверь
    map[5][5] = 4;  // Ключ
    map[5][35] = 5; // Способность (двойной прыжок)
    map[12][20] = 6; // Здоровье
    map[16][2] = 7; // Контрольная точка
}

/**
 * @brief Получает значение тайла в указанных координатах.
 * @param x Координата x тайла.
 * @param y Координата y тайла.
 * @return Значение тайла или 1, если координаты вне карты.
 */
int Level::GetTile(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) 
        return 1;
    return map[y][x];
}

/**
 * @brief Устанавливает значение тайла в указанных координатах.
 * @param x Координата x тайла.
 * @param y Координата y тайла.
 * @param value Новое значение тайла.
 */
void Level::SetTile(int x, int y, int value) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        map[y][x] = value;
    }
}