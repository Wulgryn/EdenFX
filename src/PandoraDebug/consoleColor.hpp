#ifndef CONSOLE_COLOR_HPP
#define CONSOLE_COLOR_HPP

#include "../PandoraEX/macros/whatOS.hpp"
#include <iostream>
#ifdef OS_WINDOWS
#include <windows.h>
#endif

/* REVIEW: more color
 *$ *===============================REVIEW==================================
 *$ * WHY: Enhancement
 *$ * DESCRIPTION: Add more colors to the ConsoleColor class. https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
 *$ *=======================================================================
 *$ */

namespace PandoraDebug
{
    enum class ConsoleForegroundColors
    {
#if defined(OS_WINDOWS)
        FG_BLACK = 0,
        FG_GRAY = 8,
        FG_BRIGHT_BLACK = 8,
        FG_RED = FOREGROUND_RED,
        FG_BRIGHT_RED = FOREGROUND_RED | FOREGROUND_INTENSITY,
        FG_GREEN = FOREGROUND_GREEN,
        FG_BRIGHT_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
        FG_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
        FG_BRIGHT_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
        FG_BLUE = FOREGROUND_BLUE,
        FG_BRIGHT_BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        FG_MAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
        FG_BRIGHT_MAGENTA = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        FG_CYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
        FG_BRIGHT_CYAN = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        FG_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
        FG_BRIGHT_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        FG_DEFAULT = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
#elif defined(OS_UNIX) || defined(OS_MAC)
        FG_BLACK = 30,
        FG_GRAY = 90,
        FG_BRIGHT_BLACK = 90,
        FG_RED = 31,
        FG_BRIGHT_RED = 91,
        FG_GREEN = 32,
        FG_BRIGHT_GREEN = 92,
        FG_YELLOW = 33,
        FG_BRIGHT_YELLOW = 93,
        FG_BLUE = 34,
        FG_BRIGHT_BLUE = 94,
        FG_MAGENTA = 35,
        FG_BRIGHT_MAGENTA = 95,
        FG_CYAN = 36,
        FG_BRIGHT_CYAN = 96,
        FG_WHITE = 37,
        FG_BRIGHT_WHITE = 97,
        FG_DEFAULT = 39,
#endif
    };

    enum class ConsoleBackgroundColors
    {
#if defined(OS_WINDOWS)
        BG_BLACK = 0,
        BG_GRAY = 8,
        BG_BRIGHT_BLACK = 8,
        BG_RED = BACKGROUND_RED,
        BG_BRIGHT_RED = BACKGROUND_RED | BACKGROUND_INTENSITY,
        BG_GREEN = BACKGROUND_GREEN,
        BG_BRIGHT_GREEN = BACKGROUND_GREEN | BACKGROUND_INTENSITY,
        BG_YELLOW = BACKGROUND_RED | BACKGROUND_GREEN,
        BG_BRIGHT_YELLOW = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY,
        BG_BLUE = BACKGROUND_BLUE,
        BG_BRIGHT_BLUE = BACKGROUND_BLUE | BACKGROUND_INTENSITY,
        BG_MAGENTA = BACKGROUND_RED | BACKGROUND_BLUE,
        BG_BRIGHT_MAGENTA = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
        BG_CYAN = BACKGROUND_GREEN | BACKGROUND_BLUE,
        BG_BRIGHT_CYAN = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
        BG_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
        BG_BRIGHT_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
        BG_DEFAULT = 0
#elif defined(OS_UNIX) || defined(OS_MAC)
        BG_BLACK = 40,
        BG_GRAY = 100,
        BG_BRIGHT_BLACK = 100,
        BG_RED = 41,
        BG_BRIGHT_RED = 101,
        BG_GREEN = 42,
        BG_BRIGHT_GREEN = 102,
        BG_YELLOW = 43,
        BG_BRIGHT_YELLOW = 103,
        BG_BLUE = 44,
        BG_BRIGHT_BLUE = 104,
        BG_MAGENTA = 45,
        BG_BRIGHT_MAGENTA = 105,
        BG_CYAN = 46,
        BG_BRIGHT_CYAN = 106,
        BG_WHITE = 47,
        BG_BRIGHT_WHITE = 107,
        BG_DEFAULT = 49
#endif
    };

    enum class ConsoleTextAttributes
    {
#if defined(OS_WINDOWS)
        DEFAULT = 0,
        BOLD = FOREGROUND_INTENSITY,
        DIM = FOREGROUND_INTENSITY,
        ITALIC = 3,
        UNDERLINED = 4,
        BLINK = 5,
        FAST_BLINK = 6,
        REVERSE = 7,
        HIDDEN = 8,
        STRIKETHROUGH = 9
#elif defined(OS_UNIX) || defined(OS_MAC)
        DEFAULT = 0,
        BOLD = 1,
        DIM = 2,
        ITALIC = 3,
        UNDERLINED = 4,
        BLINK = 5,
        FAST_BLINK = 6,
        REVERSE = 7,
        HIDDEN = 8,
        STRIKETHROUGH = 9
#endif
    };

    class ConsoleColor
    {
        static HANDLE ms_consoleOutput;
    public:
        static void setHandle(HANDLE consoleOutput = GetStdHandle(STD_OUTPUT_HANDLE))
        {
            ms_consoleOutput = consoleOutput;
        }


        static void set(ConsoleForegroundColors foreground = ConsoleForegroundColors::FG_DEFAULT, ConsoleBackgroundColors background = ConsoleBackgroundColors::BG_DEFAULT, ConsoleTextAttributes attributes = ConsoleTextAttributes::DEFAULT)
        {
#if defined(OS_WINDOWS)
            SetConsoleTextAttribute(ms_consoleOutput, (int)foreground | (int)background | (int)attributes);
#elif defined(OS_UNIX)
            std::cout << "\033[" << attributes << ";" << foreground << ";" << background << "m";
#elif defined(OS_MAC)
            std::cout << "\x1b[" << attributes << ";" << foreground << ";" << background << "m";
            
#endif
        }

        static void reset()
        {
#ifdef OS_WINDOWS
            SetConsoleTextAttribute(ms_consoleOutput, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#elif defined(OS_UNIX)
            std::cout << "\033[0;39;49m";
#elif defined(OS_MAC)
            std::cout << "\x1b[0;39;49m";
#endif
        }
    };
}

#endif // CONSOLE_COLOR_HPP