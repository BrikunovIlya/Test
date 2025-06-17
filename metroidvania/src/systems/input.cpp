#include "input.h"
#include <conio.h>

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

void Input::HandleGameInput(Game& game, int key) {
    switch (key) {
        case KEY_LEFT:  game.player.dx -= MOVE_ACCEL; break;
        case KEY_RIGHT: game.player.dx += MOVE_ACCEL; break;
        case KEY_JUMP:  game.player.Jump(); break;
    }
}