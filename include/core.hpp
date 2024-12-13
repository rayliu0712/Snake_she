#ifndef CORE_HPP
#define CORE_HPP

#include "tool.hpp"
#include <csignal>
#include <ncursesw/curses.h>
#define hideCur() (curs_set(0))
#define blinkCur() (curs_set(2))
#define COLUMNS (COLS / 2)

inline void clearNow()
{
    clear();
    refresh();
}

inline void waitSpaceEnter()
{
    timeout(-1);
    while (!tool::is(getch(), ' ', '\n', KEY_ENTER)) { }
}

struct Offset {
    int y, x;

    Offset(int _y = 0, int _x = 0)
        : y(_y)
        , x(_x)
    {
    }

    static inline Offset fromWall() { return Offset(2, 0); }

    static inline Offset fromMap() { return Offset(3, 1); }

    inline void mvPrint(int _y, int _x, const wstring& s)
    {
        mvaddwstr(y + _y, 2 * (x + _x), s.c_str());
    }

    inline void mvPrint(int _y, int _x, wchar_t c) { mvPrint(_y, _x, wstring { c }); }

    inline void delChar(int _y, int _x)
    {
        mvaddstr(y + _y, 2 * (x + _x), "  ");
    };

    inline void mv(int _y, int _x)
    {
        move(y + _y, 2 * (x + _x));
    }

    inline void delLine(int _y)
    {
        mv(_y, 0);
        clrtoeol();
    }
};

#endif