#include "game.h"
#include "../systems/input.h"
#include "../systems/render.h"
#include "../utils/console.h"
#include <conio.h>
#include <fstream>

Game::Game() {
    Console::SetTitle(L"Metroidvania Adventure");
    Console::HideCursor();
    musicPlaying = true;
    musicThread = std::thread(&Game::PlayBackgroundMusic, this);
}

Game::~Game() {
    musicPlaying = false;
    if (musicThread.joinable()) musicThread.join();
}

void Game::Run() {
    while (true) {
        ProcessInput();
        Update();
        Render();
        Sleep(30);
    }
}

void Game::ProcessInput() {
    Input::Process(*this);
}

void Game::Update() {
    if (state == PLAYING) {
        player.Update(level.map);
        // Add game logic here
    }
}

void Game::Render() {
    Console::ClearScreen();
    
    if (state == MAIN_MENU) {
        Render::MainMenu(*this);
    } else if (state == PLAYING) {
        Render::GameWorld(*this);
    } else if (state == PAUSED) {
        Render::PauseMenu(*this);
    }
}

void Game::PlayBackgroundMusic() {
    while (musicPlaying) {
        Beep(330, 300);
        Beep(392, 300);
        Beep(523, 300);
        Beep(392, 300);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Game::SaveGame() {
    std::ofstream file("save.dat");
    if (file) {
        file << player.x << " " << player.y << " "
             << player.health << " " << player.keys << " "
             << player.abilities["double_jump"] << " "
             << player.abilities["dash"] << " "
             << score;
    }
}

void Game::LoadGame() {
    std::ifstream file("save.dat");
    if (file) {
        file >> player.x >> player.y 
             >> player.health >> player.keys
             >> player.abilities["double_jump"]
             >> player.abilities["dash"]
             >> score;
    }
}