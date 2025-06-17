#include "level.h"

Level::Level() {
    map.resize(height, std::vector<int>(width, 0));
    GenerateTestLevel();
}

void Level::GenerateTestLevel() {
    // Borders
    for (int y = 0; y < height; y++) {
        map[y][0] = 1;
        map[y][width - 1] = 1;
    }
    for (int x = 0; x < width; x++) {
        map[0][x] = 1;
        map[height - 1][x] = 1;
    }
    
    // Platforms
    for (int x = 5; x < 15; x++) map[15][x] = 1;
    for (int x = 25; x < 35; x++) map[15][x] = 1;
    for (int x = 10; x < 30; x++) map[10][x] = 1;
    
    // Spikes
    for (int x = 17; x < 23; x++) map[16][x] = 2;
    
    // Items
    map[9][19] = 3; // Door
    map[5][5] = 4;  // Key
    map[5][35] = 5; // Ability (double jump)
    map[12][20] = 6; // Health
    map[16][2] = 7; // Checkpoint
}

int Level::GetTile(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) 
        return 1;
    return map[y][x];
}

void Level::SetTile(int x, int y, int value) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        map[y][x] = value;
    }
}