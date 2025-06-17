#pragma once
#include "../game/game.h"
#include <fstream>

class SaveSystem {
public:
    static bool Save(const Game& game, const std::string& filename);
    static bool Load(Game& game, const std::string& filename);
};