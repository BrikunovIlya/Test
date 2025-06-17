#include "render.h"
#include "../utils/console.h"

void Render::GameWorld(Game& game) {
    // Отрисовка карты
    for (int y = 0; y < game.level.height; y++) {
        for (int x = 0; x < game.level.width; x++) {
            int tile = game.level.GetTile(x, y);
            // ... отрисовка тайлов
        }
    }
    
    // Отрисовка игрока и врагов
    game.player.Draw(cameraX, cameraY);
    for (const auto& enemy : game.enemies) {
        enemy.Draw(cameraX, cameraY);
    }
    
    DrawUI(game);
}

void Render::MainMenu(Game& game) {
    Console::SetCursorPosition(10, 5);
    Console::SetColor(BRIGHT_YELLOW);
    std::cout << "METROIDVANIA ADVENTURE";
    // ... остальное меню
}

void Render::DrawUI(const Game& game) {
    // Полоска здоровья
    Console::SetCursorPosition(2, game.level.height + 1);
    Console::SetColor(BRIGHT_RED);
    std::cout << "HP: " << game.player.health;
    // ... остальной UI
}