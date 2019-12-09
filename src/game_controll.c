#include <ncurses.h>
#include "inc/game_menu.h"
#include "inc/enums.h"
#include "inc/game_base.h"
#include "inc/game_controll.h"
#include "inc/window.h"
#include "inc/ranking.h"

#define ENTER 10

int a = 0;

int GameScreenControll(GAME *game, PLAYER *player, LEVEL_INFO *level_info)
{
    int exit_game = 0, state_game, save_game;

    switch (game->state_screen)
    {
    case SCREEN_MENU:
        // SaveRankingFile(game);
        MenuScreen(game);
        break;

    case SCREEN_NOVO_JOGO:
        NewGameInit(game, player, level_info);
        game->state_screen = SCREEN_RUNNING;
        break;

    case SCREEN_CONTINUAR:
        if (level_info->time_start != 0)
            game->state_screen = SCREEN_RUNNING;
        else if (ReadSavedGame(game, player, level_info))
            game->state_screen = SCREEN_RUNNING;
        break;

    case SCREEN_RANKING:
        if (ReadRankingFile(game) == CREATED_FILE)
            InitRanking(game);
        PrintRankingFile(game);
        game->state_screen = SCREEN_MENU;
        break;

    case SCREEN_COMO_JOGAR:
        if (!a)
        {
            printf("\n\nOpcao como jogar\n\n");
            a = 1;
        }

        break;

    case SCREEN_RUNNING:

        state_game = GameRunning(game, player, level_info);

        if (state_game == GAME_PAUSE)
            game->state_screen = SCREEN_MENU;
        else if (state_game == GAME_LEVEL_FINISHED)
        {
            game->state_screen = SCREEN_LEVEL_FINISHED;
        }
        else if (state_game == GAME_TIMEOUT)
        {
            game->state_screen = SCREEN_GAME_OVER;
        }
        break;

    case SCREEN_LEVEL_FINISHED:

        player->score += CalculateScore(level_info, player);
        //player->score = 12;
        AddPlayerRanking(player, level_info, game);

        if (player->level < MAX_LEVELS - 1)
        {
            PrintScreenLevelComplete(game);
            player->level++;
            NewGameInit(game, player, level_info);
            game->state_screen = SCREEN_RUNNING;
        }

        else
            game->state_screen = SCREEN_WINNER;

        break;

    case SCREEN_WINNER:
        PrintScreenGameComplete(game);
        game->state_screen = SCREEN_MENU;
        break;

    case SCREEN_GAME_OVER:
        AddPlayerRanking(player, level_info, game);
        PrintScreenGameOver(game);
        game->state_screen = SCREEN_MENU;
        break;

    case SCREEN_FECHAR_JOGO:
        save_game = CloseGame(game, player, level_info);
        if (save_game)
            SaveGame(game, player, level_info);
        exit_game = 1;
        break;
    }
    return exit_game;
}

void GameInit(GAME *game)
{
    game->state_screen = SCREEN_MENU;
}

int CloseGame(GAME *game, PLAYER *player, LEVEL_INFO *level_info)
{
    int ch;
    int i = 23;

    char menu_list[2][4] = {"SIM", "NAO"};

    game->window = CreateNewWindow(12, 60, 70, 1);

    mvwprintw(game->window, 2, 18, "%s", "Voce deseja salvar o jogo?");

    wattron(game->window, A_STANDOUT);
    mvwprintw(game->window, 6, 23, "%s", menu_list[0]);

    wattroff(game->window, A_STANDOUT);

    mvwprintw(game->window, 6, 34, "%s", menu_list[1]);

    wrefresh(game->window);

    nodelay(game->window, FALSE);

    while ((ch = wgetch(game->window)) != ENTER)
    {
        mvwprintw(game->window, 6, i, "%s", menu_list[(i % 23) ? 1 : 0]);
        // use a variable to increment or decrement the value based on the input.
        switch (ch)
        {
        case KEY_LEFT:
        case 'a':
            i = 23;
            break;
        case KEY_RIGHT:
        case 'd':
            i = 34;
            break;
        }
        // now highlight the next item in the list.
        wattron(game->window, A_STANDOUT);

        mvwprintw(game->window, 6, i, "%s", menu_list[(i % 23) ? 1 : 0]);
        wattroff(game->window, A_STANDOUT);
    }
    endwin();

    return (i % 23) ? 0 : 1;
}

void SaveGame(GAME *game, PLAYER *player, LEVEL_INFO *level_info)
{
    FILE *arq;

    arq = fopen("/home/robert/Documents/UFRGS/Semestre_2/Algoritmos_e_Prog/trabalho_final/candy-crush-game/files_game/saved_game.bin", "wb");

    fwrite(level_info, sizeof(LEVEL_INFO), 1, arq);

    fwrite(player, sizeof(PLAYER), 1, arq);

    fwrite(game, sizeof(GAME), 1, arq);

    fclose(arq);
}

int ReadSavedGame(GAME *game, PLAYER *player, LEVEL_INFO *level_info)
{
    FILE *arq;

    arq = fopen("/home/robert/Documents/UFRGS/Semestre_2/Algoritmos_e_Prog/trabalho_final/candy-crush-game/files_game/saved_game.bin", "rb");

    fread(level_info, sizeof(LEVEL_INFO), 1, arq);

    fread(player, sizeof(PLAYER), 1, arq);

    fread(game, sizeof(GAME), 1, arq);

    fclose(arq);

    return 1;
}

void PrintScreenGameComplete(GAME *game)
{
    werase(game->window);
    wrefresh(game->window);

    game->window = CreateNewWindow(54, 180, 15, 1);

    int start_line = 15;
    int start_column = 70;

    mvwprintw(game->window, start_line, start_column, "               _____          __  __ ______              ");
    mvwprintw(game->window, ++start_line, start_column, "              / ____|   /\\   |  \\/  |  ____|             ");
    mvwprintw(game->window, ++start_line, start_column, "             | |  __   /  \\  | \\  / | |__                ");
    mvwprintw(game->window, ++start_line, start_column, "             | | |_ | / /\\ \\ | |\\/| |  __|               ");
    mvwprintw(game->window, ++start_line, start_column, "             | |__| |/ ____ \\| |  | | |____              ");
    mvwprintw(game->window, ++start_line, start_column, "  _____ ____  \\_____/_/___ \\_\\_|  |_|______|_____ ______ ");
    mvwprintw(game->window, ++start_line, start_column, " / ____/ __ \\|  \\/  |  __ \\| |    |  ____|__   __|  ____|");
    mvwprintw(game->window, ++start_line, start_column, "| |   | |  | | \\  / | |__) | |    | |__     | |  | |__   ");
    mvwprintw(game->window, ++start_line, start_column, "| |   | |  | | |\\/| |  ___/| |    |  __|    | |  |  __|  ");
    mvwprintw(game->window, ++start_line, start_column, "| |___| |__| | |  | | |    | |____| |____   | |  | |____ ");
    mvwprintw(game->window, ++start_line, start_column, " \\_____\\____/|_|  |_|_|    |______|______|  |_|  |______|");

    wrefresh(game->window);

    delay_output(3000);
}

void PrintScreenGameOver(GAME *game)
{
    werase(game->window);
    wrefresh(game->window);

    game->window = CreateNewWindow(54, 180, 15, 1);

    int start_line = 15;
    int start_column = 70;

    mvwprintw(game->window, start_line, start_column, "  _____          __  __ ______    ______      ________ _____  _ ");
    mvwprintw(game->window, ++start_line, start_column, " / ____|   /\\   |  \\/  |  ____|  / __ \\ \\    / /  ____|  __ \\| |");
    mvwprintw(game->window, ++start_line, start_column, "| |  __   /  \\  | \\  / | |__    | |  | \\ \\  / /| |__  | |__) | |");
    mvwprintw(game->window, ++start_line, start_column, "| | |_ | / /\\ \\ | |\\/| |  __|   | |  | |\\ \\/ / |  __| |  _  /| |");
    mvwprintw(game->window, ++start_line, start_column, "| |__| |/ ____ \\| |  | | |____  | |__| | \\  /  | |____| | \\ \\|_|");
    mvwprintw(game->window, ++start_line, start_column, " \\_____/_/    \\_\\_|  |_|______|  \\____/   \\/   |______|_|  \\_(_)");

    wrefresh(game->window);

    delay_output(3000);
}

void PrintScreenLevelComplete(GAME *game)
{
    werase(game->window);
    wrefresh(game->window);

    game->window = CreateNewWindow(54, 180, 15, 1);

    int start_line = 15;
    int start_column = 70;

    mvwprintw(game->window, ++start_line, start_column, "              _        ______  __          __  ______   _                         ");
    mvwprintw(game->window, ++start_line, start_column, "             | |      |  ____| \\ \\      / / |  ____| | |                       ");
    mvwprintw(game->window, ++start_line, start_column, "             | |      |  |___   \\ \\    / /  |  |___  | |                             ");
    mvwprintw(game->window, ++start_line, start_column, "             | |      |   ___|   \\ \\  / /   |   ___| | |                           ");
    mvwprintw(game->window, ++start_line, start_column, "             | |___   |  |___     \\ \\/ /    |  |___  | |____      ");
    mvwprintw(game->window, ++start_line, start_column, "  _____ ____ |______| |______|     \\__ /     |______| |______|");
    mvwprintw(game->window, ++start_line, start_column, " / ____/ __ \\|  \\/  |  __ \\| |    |  ____|__   __|  ____|");
    mvwprintw(game->window, ++start_line, start_column, "| |   | |  | | \\  / | |__) | |    | |__     | |  | |__   ");
    mvwprintw(game->window, ++start_line, start_column, "| |   | |  | | |\\/| |  ___/| |    |  __|    | |  |  __|  ");
    mvwprintw(game->window, ++start_line, start_column, "| |___| |__| | |  | | |    | |____| |____   | |  | |____ ");
    mvwprintw(game->window, ++start_line, start_column, " \\_____\\____/|_|  |_|_|    |______|______|  |_|  |______|");

    wrefresh(game->window);

    delay_output(3000);
}
