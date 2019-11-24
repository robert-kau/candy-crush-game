#include <ncurses.h>
#include "inc/game_menu.h"
#include "inc/window.h"
#include "inc/manager_keyboard.h"

#define ENTER 10

void MenuScreen(Game *game)
{
    char menu_list[5][20] = {"Novo Jogo", "Continuar",
                             "Ranking", "Como Jogar", "Fechar Jogo"};
    int ch, i = 0, width = 7;

    game->window = CreateNewWindow(7, 22, 94, 1);

    // now print all the menu items and highlight the first one
    for (i = 0; i < 5; i++)
    {
        if (i == 0)
            wattron(game->window, A_STANDOUT); // highlights the first item.
        else
            wattroff(game->window, A_STANDOUT);

        mvwprintw(game->window, i + 1, 2, "%s", menu_list[i]);
    }

    wrefresh(game->window); // update the terminal screen

    i = 0;
    ///noecho();        // disable echoing of characters on the screen
    keypad(game->window, TRUE); // enable keyboard input for the window.
    curs_set(0);                // hide the default screen cursor.

    while ((ch = wgetch(game->window)) != ENTER)
    {
        mvwprintw(game->window, i + 1, 2, "%s", menu_list[i]);
        // use a variable to increment or decrement the value based on the input.
        switch (ch)
        {
        case KEY_UP:
        case 'w':
            i--;
            i = (i < 0) ? 4 : i;
            break;
        case KEY_DOWN:
        case 's':
            i++;
            i = (i > 4) ? 0 : i;
            break;
        }
        // now highlight the next item in the list.
        wattron(game->window, A_STANDOUT);

        mvwprintw(game->window, i + 1, 2, "%s", menu_list[i]);
        wattroff(game->window, A_STANDOUT);
    }

    game->state_screen = i;

    delwin(game->window);
    endwin();
}