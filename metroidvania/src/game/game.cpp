#include "game.h"
#include "../systems/input.h"
#include "../systems/render.h"
#include "../utils/console.h"
#include <conio.h>
#include <fstream>

/**
 * @brief Конструктор класса Game.
 * 
 * Инициализирует консоль, скрывает курсор и запускает поток для фоновой музыки.
 */
Game::Game() {
    Console::SetTitle(L"Metroidvania Adventure");
    Console::HideCursor();
    musicPlaying = true;
    musicThread = std::thread(&Game::PlayBackgroundMusic, this);
}

/**
 * @brief Деструктор класса Game.
 * 
 * Останавливает воспроизведение музыки и завершает поток, если он активен.
 */
Game::~Game() {
    musicPlaying = false;
    if (musicThread.joinable()) musicThread.join();
}

/**
 * @brief Запускает основной игровой цикл.
 * 
 * Обрабатывает ввод, обновляет состояние и отрисовывает игру с задержкой 30 мс.
 */
void Game::Run() {
    while (true) {
        ProcessInput();
        Update();
        Render();
        Sleep(30);
    }
}

/**
 * @brief Обрабатывает пользовательский ввод.
 * 
 * Вызывает систему ввода для обработки действий игрока.
 */
void Game::ProcessInput() {
    Input::Process(*this);
}

/**
 * @brief Обновляет состояние игры.
 * 
 * Обновляет состояние игрока, если игра в состоянии PLAYING.
 */
void Game::Update() {
    if (state == PLAYING) {
        player.Update(level.map);
        // Добавить игровую логику здесь
    }
}

/**
 * @brief Отрисовывает текущее состояние игры.
 * 
 * Очищает экран и отрисовывает главное меню, игровой мир или меню паузы в зависимости от состояния.
 */
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

/**
 * @brief Воспроизводит фоновую музыку.
 * 
 * Проигрывает последовательность звуков в цикле, пока флаг musicPlaying активен.
 */
void Game::PlayBackgroundMusic() {
    while (musicPlaying) {
        Beep(330, 300);
        Beep(392, 300);
        Beep(523, 300);
        Beep(392, 300);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

/**
 * @brief Сохраняет текущее состояние игры в файл.
 * 
 * Записывает позицию игрока, здоровье, ключи, способности и счет в файл save.dat.
 */
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

/**
 * @brief Загружает состояние игры из файла.
 * 
 * Читает позицию игрока, здоровье, ключи, способности и счет из файла save.dat.
 */
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