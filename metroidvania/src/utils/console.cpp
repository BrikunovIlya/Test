#include "console.h"

/**
 * @brief Устанавливает цвет текста и фона в консоли.
 * @param text Цвет текста (из перечисления ConsoleColor).
 * @param background Цвет фона (из перечисления ConsoleColor, по умолчанию BLACK).
 */
void Console::SetColor(ConsoleColor text, ConsoleColor background) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)((background << 4) | text));
}

/**
 * @brief Очищает экран консоли.
 */
void Console::ClearScreen() {
    system("cls");
}

/**
 * @brief Устанавливает позицию курсора в консоли.
 * @param x Координата x позиции курсора.
 * @param y Координата y позиции курсора.
 */
void Console::SetCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/**
 * @brief Скрывает курсор в консоли.
 */
void Console::HideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

/**
 * @brief Устанавливает заголовок окна консоли.
 * @param title Текст заголовка (в формате wide string).
 */
void Console::SetTitle(const wchar_t* title) {
    SetConsoleTitle(title);
}