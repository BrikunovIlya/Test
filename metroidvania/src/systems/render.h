#pragma once
#include "../game/game.h"

namespace Render {
    void GameWorld(Game& game);
    void MainMenu(Game& game);
    void PauseMenu(Game& game);
    void DrawUI(const Game& game);
}