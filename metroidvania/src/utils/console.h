#pragma once

#include <windows.h>

enum ConsoleColor {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    YELLOW = 6,
    WHITE = 7,
    GRAY = 8,
    BRIGHT_BLUE = 9,
    BRIGHT_GREEN = 10,
    BRIGHT_CYAN = 11,
    BRIGHT_RED = 12,
    BRIGHT_MAGENTA = 13,
    BRIGHT_YELLOW = 14,
    BRIGHT_WHITE = 15
};

namespace Console {
    void SetColor(ConsoleColor text, ConsoleColor background = BLACK);
    void ClearScreen();
    void SetCursorPosition(int x, int y);
    void HideCursor();
    void SetTitle(const wchar_t* title);
}