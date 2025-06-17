#pragma once
#include <windows.h>

/**
 * @brief Перечисление цветов консоли.
 * 
 * Определяет доступные цвета для текста и фона в консоли.
 */
enum ConsoleColor {
    BLACK = 0,          /**< Черный цвет. */
    BLUE = 1,           /**< Синий цвет. */
    GREEN = 2,          /**< Зеленый цвет. */
    CYAN = 3,           /**< Циановый цвет. */
    RED = 4,            /**< Красный цвет. */
    MAGENTA = 5,        /**< Пурпурный цвет. */
    YELLOW = 6,         /**< Желтый цвет. */
    WHITE = 7,          /**< Белый цвет. */
    GRAY = 8,           /**< Серый цвет. */
    BRIGHT_BLUE = 9,    /**< Ярко-синий цвет. */
    BRIGHT_GREEN = 10,  /**< Ярко-зеленый цвет. */
    BRIGHT_CYAN = 11,   /**< Ярко-циановый цвет. */
    BRIGHT_RED = 12,    /**< Ярко-красный цвет. */
    BRIGHT_MAGENTA = 13,/**< Ярко-пурпурный цвет. */
    BRIGHT_YELLOW = 14, /**< Ярко-желтый цвет. */
    BRIGHT_WHITE = 15   /**< Ярко-белый цвет. */
};

/**
 * @brief Пространство имен для управления консолью.
 * 
 * Содержит функции для настройки цвета, очистки экрана, установки позиции курсора,
 * скрытия курсора и изменения заголовка консоли.
 */
namespace Console {
    /**
     * @brief Устанавливает цвет текста и фона в консоли.
     * @param text Цвет текста (из перечисления ConsoleColor).
     * @param background Цвет фона (из перечисления ConsoleColor, по умолчанию BLACK).
     */
    void SetColor(ConsoleColor text, ConsoleColor background = BLACK);

    /**
     * @brief Очищает экран консоли.
     */
    void ClearScreen();

    /**
     * @brief Устанавливает позицию курсора в консоли.
     * @param x Координата x позиции курсора.
     * @param y Координата y позиции курсора.
     */
    void SetCursorPosition(int x, int y);

    /**
     * @brief Скрывает курсор в консоли.
     */
    void HideCursor();

    /**
     * @brief Устанавливает заголовок окна консоли.
     * @param title Текст заголовка (в формате wide string).
     */
    void SetTitle(const wchar_t* title);
}