#include <ncursesw/ncurses.h>
#include <locale.h>
#include <time.h>
#include <stdlib.h>

#include "inc/window.h"

static void InitWindowColors();

void InitWindow()
{

    srand(time(NULL));

    initscr();

    InitWindowColors();

    noecho();

    curs_set(FALSE);

    cbreak();

    scrollok(stdscr, TRUE);

    refresh();
}

static void InitWindowColors()
{
    start_color();

    use_default_colors();

    init_pair(1, COLOR_RED, -1);
    init_pair(2, COLOR_BLUE, -1);
    init_pair(3, -1, COLOR_YELLOW);
}

WINDOW *CreateNewWindow(uint16_t lines, uint16_t columns,
                        uint16_t start_x, uint16_t start_y)
{
    WINDOW *window;
    window = newwin(lines, columns, start_y, start_x);
    box(window, 0, 0);

    return window;
}
