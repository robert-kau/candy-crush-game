#include <ncurses.h>
#include "inc/game_structs.h"
#include "inc/enums.h"
#include "inc/window.h"
#include "inc/game_base.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "inc/manager_keyboard.h"

void NewGameInit(GAME *game, PLAYER *player, LEVEL_INFO *level_info)
{

    if (game->state_screen == SCREEN_NOVO_JOGO)
    {
        echo();

        game->window = CreateNewWindow(7, 40, 90, 1);
        mvwprintw(game->window, 2, 8, "Digite o nome do jogador");
        mvwprintw(game->window, 4, 2, "->");
        wrefresh(game->window);

        mvwgetnstr(game->window, 4, 5, player->name, MAX_LENGTH_PLAYER_NAME);

        noecho();

        player->level = INITIAL_LEVEL;
        player->score = INITIAL_SCORE;
    }

    level_info->lin = INIT_POS_CURSOR;
    level_info->col = INIT_POS_CURSOR;

    level_info->lin_old = level_info->lin;
    level_info->col_old = level_info->col;

    ReadFileLevel(player, level_info);

    level_info->time_start = time(NULL);

    level_info->time_calib = 0;

    level_info->n_space = 0;

    level_info->n_combintions = 0;

    delwin(game->window);
    endwin();
}

int GameRunning(GAME *game, PLAYER *player, LEVEL_INFO *level_info)
{
    if (level_info->time_calib > 0)
    {
        level_info->time_start = time(NULL);

        level_info->time_rest = (TIME_FIRST_LEVEL + (player->level * TIME_OTHER_LEVEL)) - level_info->time_calib;

        level_info->time_start -= level_info->time_rest;
    }

    clear();

    game->window = CreateNewWindow(54, 180, 15, 1);
    mvwprintw(game->window, 2, 8, "Jogador: %s", player->name);
    mvwprintw(game->window, 3, 8, "Combinacoes para proximo nivel: %d", level_info->combinations_next_level);
    wrefresh(game->window);

    PrintColorMatrix(game, level_info->tabuleiro);

    PrintCursorMatrix(game, (((sqrt(PIXELS_PIECE) + SPACE_SIZE) * level_info->lin) + POS_TAB_LIN - 1), (((sqrt(PIXELS_PIECE) + SPACE_SIZE) * level_info->col) + POS_TAB_COL - 1));

    keypad(game->window, TRUE); // enable keyboard input for the window.
    curs_set(0);                // hide the default screen cursor.

    nodelay(game->window, TRUE);

    UpdateTime(game, level_info, player);

    while (((level_info->ch = wgetch(game->window)) != ESC) && level_info->time_left > 0 &&
           level_info->n_combintions < level_info->combinations_next_level)
    {
        UpdateTime(game, level_info, player);

        switch (level_info->ch)
        {
        case KEY_UP:
        case 'w':
            if (level_info->lin > EDGE)
                level_info->lin--;
            break;

        case KEY_DOWN:
        case 's':
            if (level_info->lin <= MAP_LINES - 3 * EDGE)
                level_info->lin++;
            break;

        case 'a':
        case KEY_LEFT:
            if (level_info->col > EDGE)
                level_info->col--;
            break;

        case 'd':
        case KEY_RIGHT:
            if (level_info->col <= MAP_COL - 3 * EDGE)
                level_info->col++;
            break;
        case SPACE:
            level_info->n_space++;

            if (level_info->n_space >= 2)
            {
                level_info->n_space = 0;
                ChangePositionInMatrix(level_info);
                UpdateMatrixScreen(game, player, level_info);
                UpdateCursorMatrix(game, level_info);

                FindCombinationMatrix(game, player, level_info);
            }

            FindCombinationMatrix(game, player, level_info);
            level_info->first_select_col = level_info->col;
            level_info->first_select_lin = level_info->lin;
            UpdateMatrixScreen(game, player, level_info);
            UpdateCursorMatrix(game, level_info);

            break;
        }

        if (level_info->lin_old != level_info->lin || level_info->col_old != level_info->col)
        {
            level_info->lin_old = level_info->lin;
            level_info->col_old = level_info->col;

            UpdateMatrixScreen(game, player, level_info);

            if (level_info->n_space > 0)
                UpdateCursorMatrix(game, level_info);
        }
        nodelay(game->window, TRUE);
    }

    werase(game->window);
    wrefresh(game->window);
    level_info->time_calib = level_info->time_left;

    if (level_info->time_left <= 0)
        return GAME_TIMEOUT;
    else if (level_info->n_combintions >= level_info->combinations_next_level)
        return GAME_LEVEL_FINISHED;
    else
        return GAME_PAUSE;
}

void ReadFileLevel(PLAYER *player, LEVEL_INFO *level_info)
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
                if (fscanf(arq, "%c", &level_info->tabuleiro[lin][col]) > 0)
                {
                    if (level_info->tabuleiro[lin][col] == '\n')
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

void UpdateMatrixScreen(GAME *game, PLAYER *player, LEVEL_INFO *level_info)
{
    werase(game->window);

    game->window = CreateNewWindow(54, 180, 15, 1);

    keypad(game->window, TRUE);

    mvwprintw(game->window, 2, 8, "Jogador: %s", player->name);
    mvwprintw(game->window, 3, 8, "Combinacoes para proximo nivel: %d", level_info->combinations_next_level);
    //mvwprintw(game->window, 4, 8, "Linha: %d", level_info->lin);
    //mvwprintw(game->window, 5, 8, "Coluna: %d", level_info->col);
    //mvwprintw(game->window, 6, 8, "Space: %d", level_info->n_space);
    //mvwprintw(game->window, 7, 8, "Lin-col: %c", level_info->tabuleiro[level_info->lin][level_info->col]);
    //mvwprintw(game->window, 8, 8, "Lin-col-first: %c", level_info->tabuleiro[level_info->first_select_lin][level_info->first_select_col]);
    mvwprintw(game->window, 9, 8, "Combinacoe feitas: %d", level_info->n_combintions);

    PrintColorMatrix(game, level_info->tabuleiro);

    PrintCursorMatrix(game, (((sqrt(PIXELS_PIECE) + SPACE_SIZE) * level_info->lin) + POS_TAB_LIN - 1), (((sqrt(PIXELS_PIECE) + SPACE_SIZE) * level_info->col) + POS_TAB_COL - 1));

    UpdateTime(game, level_info, player);

    wrefresh(game->window);
}

void UpdateCursorMatrix(GAME *game, LEVEL_INFO *level_info)
{
    PrintCursorMatrix(game, (((sqrt(PIXELS_PIECE) + SPACE_SIZE) * level_info->first_select_lin) + POS_TAB_LIN - 1), (((sqrt(PIXELS_PIECE) + SPACE_SIZE) * level_info->first_select_col) + POS_TAB_COL - 1));
}

void ChangePositionInMatrix(LEVEL_INFO *level_info)
{
    char aux_piece;

    aux_piece = level_info->tabuleiro[level_info->lin][level_info->col];

    level_info->tabuleiro[level_info->lin][level_info->col] = level_info->tabuleiro[level_info->first_select_lin][level_info->first_select_col];

    level_info->tabuleiro[level_info->first_select_lin][level_info->first_select_col] = aux_piece;
}

void FindCombinationMatrix(GAME *game, PLAYER *player, LEVEL_INFO *level_info)
{
    int lin, col;
    for (lin = EDGE; lin < MAP_LINES - EDGE; lin++)
        for (col = EDGE; col < MAP_COL - EDGE * 3; col++)
        {
            if (level_info->tabuleiro[lin][col] != 'Z')
                if (level_info->tabuleiro[lin][col] == level_info->tabuleiro[lin][col + 1])
                    if (level_info->tabuleiro[lin][col] == level_info->tabuleiro[lin][col + 2])
                    {
                        level_info->tabuleiro[lin][col] = 'Z';
                        level_info->tabuleiro[lin][col + 1] = 'Z';
                        level_info->tabuleiro[lin][col + 2] = 'Z';
                        level_info->n_combintions++;
                        CompleteMatrix(game, player, level_info);
                    }
        }
}

void CompleteMatrix(GAME *game, PLAYER *player, LEVEL_INFO *level_info)
{
    int lin, col, lin_aux = 0, lin_aux2 = 0, aux_piece;

    for (col = EDGE; col < MAP_COL - EDGE; col++)
        for (lin = MAP_LINES - 1 - 1; lin >= EDGE; lin--)
        {
            if (level_info->tabuleiro[lin][col] == 'Z')
            {
                for (lin_aux2 = lin; lin_aux2 > EDGE; lin_aux2--)
                {
                    aux_piece = level_info->tabuleiro[lin_aux2][col];
                    level_info->tabuleiro[lin_aux2][col] = level_info->tabuleiro[lin_aux2 - 1][col];
                    level_info->tabuleiro[lin_aux2 - 1][col] = aux_piece;

                    //level_info->tabuleiro[lin_aux2][col] = level_info->tabuleiro[lin_aux2 - 1][col];

                    UpdateMatrixScreen(game, player, level_info);

                    delay_output(250);
                }
                level_info->tabuleiro[lin_aux2][col] = RandomPiece();
            }
        }
    FindCombinationMatrix(game, player, level_info);
}

char RandomPiece(void)
{
    int n_rand;
    char rand_piece;

    n_rand = MIN + (rand() % (MAX - MIN + 1));

    if (n_rand < 250)
        rand_piece = 'R';

    if (250 <= n_rand && n_rand < 500)
        rand_piece = 'G';

    if (500 <= n_rand && n_rand < 750)
        rand_piece = 'B';

    if (750 <= n_rand && n_rand < 1000)
        rand_piece = 'C';

    return rand_piece;
}

int UpdateTime(GAME *game, LEVEL_INFO *level_info, PLAYER *player)
{
    int time_aux, there_is_time, time_aux2;

    time_aux = (level_info->time_start + TIME_FIRST_LEVEL + (player->level * TIME_OTHER_LEVEL)) - time(NULL);

    if (time_aux >= 0)
    {
        level_info->time_left = time_aux;
        there_is_time = 1;
    }
    else
        there_is_time = 0;

    if (game->state_screen == SCREEN_RUNNING)
    {
        mvwprintw(game->window, 10, 8, "Tempo restante: %3ds", level_info->time_left);
        //mvwprintw(game->window, 11, 8, "Tempo r: %3ds", level_info->time_rest);
        wrefresh(game->window);
    }

    return there_is_time;
}

int CalculateScore(LEVEL_INFO *level_info, PLAYER *player)
{
    int seconds, points;

    seconds = TIME_FIRST_LEVEL + (player->level * TIME_OTHER_LEVEL) - level_info->time_left;

    points = (level_info->n_combintions * POINTS_FOR_COMB) - (POINTS_FOR_S * seconds);

    if (points < 0)
        points = 0;

    return points;
}