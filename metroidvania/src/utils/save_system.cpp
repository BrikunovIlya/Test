#include "save_system.h"

/**
 * @brief Сохраняет текущее состояние игры в указанный файл.
 * @param game Экземпляр класса Game, содержащий данные об игре.
 * @param filename Имя файла для сохранения.
 * @return true, если сохранение успешно, иначе false.
 */
bool SaveSystem::Save(const Game& game, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) return false;
    
    // Сериализация состояния игры
    file << game.player.x << " " << game.player.y << "\n";
    file << game.level.width << " " << game.level.height << "\n";
    
    return true;
}

/**
 * @brief Загружает состояние игры из указанного файла.
 * @param game Экземпляр класса Game, в который загружаются данные.
 * @param filename Имя файла для загрузки.
 * @return true, если загрузка успешна, иначе false.
 */
bool SaveSystem::Load(Game& game, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) return false;
    
    // Десериализация
    file >> game.player.x >> game.player.y;
    
    return true;
}