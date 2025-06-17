#pragma once
#include "../game/game.h"

namespace Input {
    void Process(Game& game);
    void HandleGameInput(Game& game);
    void HandleMenuInput(Game& game);
}