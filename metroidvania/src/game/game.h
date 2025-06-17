#pragma once
#include "level.h"
#include "../entities/player.h"
#include <vector>
#include <atomic>
#include <thread>

class Game {
public:
    enum State { MAIN_MENU, PLAYING, PAUSED };
    
    State state = MAIN_MENU;
    Level level;
    Player player{2.0f, 16.0f};
    int score = 0;
    std::atomic<bool> musicPlaying{false};
    std::thread musicThread;

    Game();
    ~Game();
    
    void Run();
    void ProcessInput();
    void Update();
    void Render();
    void PlayBackgroundMusic();
    void SaveGame();
    void LoadGame();
};