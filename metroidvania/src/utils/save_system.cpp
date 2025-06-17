#include "save_system.h"

bool SaveSystem::Save(const Game& game, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) return false;
    
    // Сериализация состояния игры
    file << game.player.x << " " << game.player.y << "\n";
    file << game.level.width << " " << game.level.height << "\n";
    
    return true;
}

bool SaveSystem::Load(Game& game, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) return false;
    
    // Десериализация
    file >> game.player.x >> game.player.y;
    
    return true;
}