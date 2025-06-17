#pragma once
#include "../game/game.h"

/**
 * @brief Пространство имен для обработки пользовательского ввода.
 * 
 * Содержит функции для обработки ввода в различных состояниях игры.
 */
namespace Input {
    /**
     * @brief Обрабатывает пользовательский ввод в зависимости от состояния игры.
     * @param game Экземпляр класса Game, содержащий данные об игре.
     */
    void Process(Game& game);

    /**
     * @brief Обрабатывает ввод во время игрового процесса.
     * @param game Экземпляр класса Game, содержащий данные об игре.
     * @param key Код нажатой клавиши.
     */
    void HandleGameInput(Game& game, int key);

    /**
     * @brief Обрабатывает ввод в главном меню.
     * @param game Экземпляр класса Game, содержащий данные об игре.
     * @param key Код нажатой клавиши.
     */
    void HandleMenuInput(Game& game, int key);
}