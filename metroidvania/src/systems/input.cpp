#include "input.h"
#include <conio.h>

/**
 * @brief Обрабатывает пользовательский ввод в зависимости от состояния игры.
 * @param game Экземпляр класса Game, содержащий данные об игре.
 */
void Input::Process(Game& game) {
    if (_kbhit()) {
        int key = _getch();
        
        switch (game.state) {
            case Game::MAIN_MENU:
                HandleMenuInput(game, key);
                break;
            case Game::PLAYING:
                HandleGameInput(game, key);
                break;
        }
    }
}

/**
 * @brief Обрабатывает ввод во время игрового процесса.
 * @param game Экземпляр класса Game, содержащий данные об игре.
 * @param key Код нажатой клавиши.
 */
void Input::HandleGameInput(Game& game, int key) {
    switch (key) {
        case KEY_LEFT:  game.player.dx -= MOVE_ACCEL; break;
        case KEY_RIGHT: game.player.dx += MOVE_ACCEL; break;
        case KEY_JUMP:  game.player.Jump(); break;
    }
}