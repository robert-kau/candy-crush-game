#include <ncurses.h>
#include "inc/game_structs.h"
#include "inc/enums.h"
#include "inc/window.h"
#include "inc/game_base.h"
#include <stdlib.h>

PLAYER *NewGameInit(GAME *game)
{
    PLAYER *player = malloc(sizeof(PLAYER));

    echo();

    game->window = CreateNewWindow(7, 40, 90, 1);

    mvwprintw(game->window, 2, 8, "Digite o nome do jogador");
    mvwprintw(game->window, 4, 2, "->");
    wrefresh(game->window);

    mvwgetnstr(game->window, 4, 5, player->name, 30);

    player->level = 0;
    player->score = 0;

    noecho();

    delwin(game->window);
    endwin();

    return player;
}

void GameRunning(GAME *game, PLAYER *player)
{
    char tabuleiro[7][11];
    char *ptr_matrix = &tabuleiro[0][0];

    clear();
    game->window = CreateNewWindow(54, 180, 15, 1);
    mvwprintw(game->window, 2, 8, "Jogador: %s", player->name);
    wrefresh(game->window);

    ReadFileLevel(player, ptr_matrix);
}

char ReadFileLevel(PLAYER *player, char *ptr_matrix)
{
    FILE *arq;
}