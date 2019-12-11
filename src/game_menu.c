#include <ncurses.h>
#include "inc/game_menu.h"
#include "inc/window.h"
#include "inc/manager_keyboard.h"

#define ENTER 10

void MenuScreen(GAME *game)
{
    char menu_list[5][20] = {"Novo Jogo", "Continuar",
                             "Ranking", "Como Jogar", "Fechar Jogo"};
    int ch, i = 0;

    clear();

    werase(game->window);
    wrefresh(game->window);

    game->window = CreateNewWindow(7, 22, 94, 1);

    for (i = 0; i < 5; i++)
    {
        if (i == 0)
            wattron(game->window, A_STANDOUT);
        else
            wattroff(game->window, A_STANDOUT);

        mvwprintw(game->window, i + 1, 2, "%s", menu_list[i]);
    }

    wrefresh(game->window);

    i = 0;

    keypad(game->window, TRUE);
    curs_set(0);

    while ((ch = wgetch(game->window)) != ENTER)
    {
        mvwprintw(game->window, i + 1, 2, "%s", menu_list[i]);

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

        wattron(game->window, A_STANDOUT);

        mvwprintw(game->window, i + 1, 2, "%s", menu_list[i]);
        wattroff(game->window, A_STANDOUT);
    }

    game->state_screen = i;

    delwin(game->window);
    endwin();
}