#include <ncurses.h>
#include "inc/game_structs.h"
#include "inc/enums.h"
#include "inc/window.h"
#include "inc/game_base.h"
#include <stdlib.h>
#include <math.h>

void NewGameInit(GAME *game, PLAYER *player)
{
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
}

void GameRunning(GAME *game, PLAYER *player, LEVEL_INFO *level_info)
{
    char tabuleiro[MAP_LINES][MAP_COL];
    int lin = EDGE, col = EDGE, lin_old, col_old;
    int ch;

    ReadFileLevel(player, tabuleiro, level_info);

    clear();

    game->window = CreateNewWindow(54, 180, 15, 1);
    mvwprintw(game->window, 2, 8, "Jogador: %s", player->name);
    mvwprintw(game->window, 3, 8, "Combinacoes para proximo nivel: %d", level_info->combinations_next_level);
    wrefresh(game->window);

    PrintColorMatrix(game, tabuleiro);

    PrintCursorMatrix(game, (((sqrt(PIXELS_PIECE) + SPACE_SIZE) * INIT_POS_CURSOR) + POS_TAB_LIN - 1), (((sqrt(PIXELS_PIECE) + SPACE_SIZE) * INIT_POS_CURSOR) + POS_TAB_COL - 1));

    keypad(game->window, TRUE); // enable keyboard input for the window.
    curs_set(0);                // hide the default screen cursor.

    lin_old = lin;
    col_old = col;

    while ((ch = wgetch(game->window)) != ESC)
    {
        switch (ch)
        {
        case KEY_UP:
        case 'w':
            if (lin > EDGE)
                lin--;
            break;

        case KEY_DOWN:
        case 's':
            if (lin <= MAP_LINES - 3 * EDGE)
                lin++;
            break;

        case 'a':
        case KEY_LEFT:
            if (col > EDGE)
                col--;
            break;

        case 'd':
        case KEY_RIGHT:
            if (col <= MAP_COL - 3 * EDGE)
                col++;
            break;
        }

        if (lin_old != lin || col_old != col)
        {
            lin_old = lin;
            col_old = col;

            werase(game->window);

            game->window = CreateNewWindow(54, 180, 15, 1);

            keypad(game->window, TRUE);

            mvwprintw(game->window, 2, 8, "Jogador: %s", player->name);
            mvwprintw(game->window, 3, 8, "Combinacoes para proximo nivel: %d", level_info->combinations_next_level);

            PrintColorMatrix(game, tabuleiro);

            PrintCursorMatrix(game, (((sqrt(PIXELS_PIECE) + SPACE_SIZE) * lin) + POS_TAB_LIN - 1), (((sqrt(PIXELS_PIECE) + SPACE_SIZE) * col) + POS_TAB_COL - 1));

            wrefresh(game->window);
        }
    }

    werase(game->window);
    wrefresh(game->window);
}

void ReadFileLevel(PLAYER *player, char tabuleiro[MAP_LINES][MAP_COL], LEVEL_INFO *level_info)
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
        fscanf(arq, "%s", time);
        level_info->combinations_next_level = atoi(time);
        for (lin = 0; lin < MAP_LINES; lin++)
        {
            for (col = 0; col < MAP_COL; col++)
                if (fscanf(arq, "%c", &tabuleiro[lin][col]) > 0)
                {
                    if (tabuleiro[lin][col] == '\n')
                        col--;
                }
                else
                    printf("\nErro na leitura!!\n");
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

    for (i = 0; i < PIXELS_PIECE; i++, j++)
    {
        if (j > (sqrt(PIXELS_PIECE) - 1))
        {
            j = 0;
            k++;
        }

        for (lin = j, lin_aux = 0; lin_aux < MAP_LINES; lin_aux++, lin += sqrt(PIXELS_PIECE) + SPACE_SIZE)
        {
            for (col = k, col_aux = 0; col_aux < MAP_COL; col_aux++, col += sqrt(PIXELS_PIECE) + SPACE_SIZE)
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
        }
    }
    wrefresh(game->window);
}

void PrintCursorMatrix(GAME *game, int lin, int col)
{
    int lin_aux, col_aux;

    start_color();

    init_pair(WHITE, COLOR_BLACK, COLOR_WHITE);

    wattron(game->window, COLOR_PAIR(WHITE));

    for (lin_aux = lin; lin_aux < (lin + (sqrt(PIXELS_PIECE) + SPACE_SIZE)); lin_aux++)
    {
        for (col_aux = col; col_aux < (col + (sqrt(PIXELS_PIECE) + SPACE_SIZE)); col_aux++)
        {
            if (lin_aux >= lin + 1 && lin_aux <= lin + 4 && (col_aux == col || col_aux == col + 5))
                mvwprintw(game->window, lin_aux, col_aux, "%s", " ");
            else if (lin_aux == lin)
                mvwprintw(game->window, lin_aux, col_aux, "%s", " ");
            else if (lin_aux == lin + 4)
                mvwprintw(game->window, lin_aux, col_aux, "%s", " ");
        }
    }
    wrefresh(game->window);
}