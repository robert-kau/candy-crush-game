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

    mvwgetnstr(game->window, 4, 5, player->name, MAX_LENGTH_PLAYER_NAME);

    player->level = INITIAL_LEVEL;
    player->score = INITIAL_SCORE;

    noecho();

    delwin(game->window);
    endwin();

    return player;
}

void GameRunning(GAME *game, PLAYER *player)
{
    char tabuleiro[MAP_LINES][MAP_COL];
    int lin, col;

    clear();
    game->window = CreateNewWindow(54, 180, 15, 1);
    mvwprintw(game->window, 2, 8, "Jogador: %s", player->name);
    wrefresh(game->window);

    ReadFileLevel(player, tabuleiro);
    /*
    for (lin = 0; lin < MAP_LINES; lin++)
    {
        for (col = 0; col < MAP_COL; col++)
            mvwprintw(game->window, lin + 5, col + 3, "%c", tabuleiro[lin][col]);
    }
    wrefresh(game->window);
*/
    PrintColorMatrix(game, tabuleiro);
}

void ReadFileLevel(PLAYER *player, char tabuleiro[MAP_LINES][MAP_COL])
{
    FILE *arq;
    char name_file[150];
    int lin, col;
    char time[20];

    sprintf(name_file, "/home/robert/Documents/UFRGS/Semestre_2/Algoritmos_e_Prog/trabalho_final/candy-crush-game/Niveis/nivel-%d.txt", player->level);

    if ((arq = fopen(name_file, "r")) == NULL)
        printf("\n\nErro ao abrir arquivo.\n\n");
    else
    {
        fscanf(arq, "%s\n", time);
        for (lin = 0; lin < MAP_LINES; lin++)
        {
            for (col = 0; col < MAP_COL; col++)
                if (!fscanf(arq, "%c", &tabuleiro[lin][col]))
                    break;
        }
    }
}

void PrintColorMatrix(GAME *game, char tabuleiro[MAP_LINES][MAP_COL])
{
    int lin, col, lin_aux, col_aux, i, j = 0, k = 0;

    start_color();

    init_pair(RED, COLOR_BLACK, COLOR_RED);
    init_pair(GREEN, COLOR_BLACK, COLOR_GREEN);
    init_pair(CYAN, COLOR_BLACK, COLOR_CYAN);
    init_pair(BLUE, COLOR_BLACK, COLOR_BLUE);
    init_pair(YELLOW, COLOR_BLACK, COLOR_YELLOW);

    //wattron(game->window, COLOR_PAIR(RED));
    //mvwprintw(game->window, lin + 5, col + 3, "%s", " ");

    for (i = 0; i < 4; i++)
    {
        switch (i)
        {
        case 0:
            j = 0;
            k = 0;
            break;
        case 1:
            j = 0;
            k = 1;
            break;
        case 2:
            j = 1;
            k = 0;
            break;
        case 3:
            j = 1;
            k = 1;
            break;
        }

        for (lin = j, lin_aux = 0; lin_aux < MAP_LINES; lin_aux++, lin += PIECE_SIZE + EDGE_SIZE)
        {
            for (col = k, col_aux = 0; col_aux < MAP_COL; col_aux++, col += PIECE_SIZE + EDGE_SIZE)
            {
                switch (tabuleiro[lin_aux][col_aux])
                {
                case 'R':
                    wattron(game->window, COLOR_PAIR(RED));
                    mvwprintw(game->window, lin + POS_TAB_LIN, col + POS_TAB_COL, "%s", " ");
                    break;
                case 'G':
                    wattron(game->window, COLOR_PAIR(GREEN));
                    mvwprintw(game->window, lin + POS_TAB_LIN, col + POS_TAB_COL, "%s", " ");
                    break;
                case 'B':
                    wattron(game->window, COLOR_PAIR(BLUE));
                    mvwprintw(game->window, lin + POS_TAB_LIN, col + POS_TAB_COL, "%s", " ");
                    break;
                case 'C':
                    wattron(game->window, COLOR_PAIR(CYAN));
                    mvwprintw(game->window, lin + POS_TAB_LIN, col + POS_TAB_COL, "%s", " ");
                    break;
                case 'x':
                    wattron(game->window, COLOR_PAIR(YELLOW));
                    mvwprintw(game->window, lin + POS_TAB_LIN, col + POS_TAB_COL, "%s", " ");
                    break;
                }
            }
            // mvwprintw(game->window, lin + 5, col + 3, "%c", tabuleiro[lin][col]);
        }
    }

    wrefresh(game->window);
}