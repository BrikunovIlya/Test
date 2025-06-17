#pragma once
#include <vector>

class Level {
public:
    std::vector<std::vector<int>> map;
    int width = 60;
    int height = 30;

    Level();
    void GenerateTestLevel();
    int GetTile(int x, int y) const;
    void SetTile(int x, int y, int value);
};