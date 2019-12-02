#include <ncurses.h>
#include "inc/game_menu.h"
#include "inc/enums.h"
#include "inc/game_base.h"
#include "inc/game_controll.h"
#include "inc/window.h"

int a = 0;

int GameScreenControll(GAME *game, PLAYER *player, LEVEL_INFO *level_info)
{
    int exit_game = 0, state_game;

    switch (game->state_screen)
    {
    case SCREEN_MENU:
        MenuScreen(game);
        break;

    case SCREEN_NOVO_JOGO:
        NewGameInit(game, player, level_info);
        game->state_screen = SCREEN_RUNNING;
        break;

    case SCREEN_CONTINUAR:
        game->state_screen = SCREEN_RUNNING;
        break;

    case SCREEN_RANKING:
        if (!a)
        {
            printf("\n\nOpcao ranking\n\n");
            a = 1;
        }

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
        PrintScreenGameOver(game);
        game->state_screen = SCREEN_MENU;
        break;

    case SCREEN_FECHAR_JOGO:
        CloseGame(game, player, level_info);
        exit_game = 1;
        break;
    }
    return exit_game;
}

void GameInit(GAME *game)
{
    game->state_screen = SCREEN_MENU;
}

void CloseGame(GAME *game, PLAYER *player, LEVEL_INFO *level_info)
{
}

void PrintScreenGameComplete(GAME *game)
{
    clear();

    game->window = CreateNewWindow(54, 180, 15, 1);

    int start_line = (33 - 12) / 2;
    int start_column = (83 - 57) / 2;

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
    game->window = CreateNewWindow(54, 180, 15, 1);

    int start_line = (33 - 12) / 2;
    int start_column = (83 - 57) / 2;

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
    clear();

    game->window = CreateNewWindow(54, 180, 15, 1);

    int start_line = (33 - 12) / 2;
    int start_column = (83 - 57) / 2;

    mvwprintw(game->window, start_line, start_column, "                _        ______  __          __  ______   __                          ");
    mvwprintw(game->window, ++start_line, start_column, "             | |      |  ____| \\ \\      / / |  ____| | |                       ");
    mvwprintw(game->window, ++start_line, start_column, "             | |      |  |___   \\ \\    / /  |  |___  | |                             ");
    mvwprintw(game->window, ++start_line, start_column, "             | |      |   ___|   \\ \\  / /   |   ___| | |                           ");
    mvwprintw(game->window, ++start_line, start_column, "             | |___   |  |___     \\ \\/ /    |  |___  | |___                  ");
    mvwprintw(game->window, ++start_line, start_column, "  _____ ____ |______| |______|     \\__ /     |______| |______|                         ");
    mvwprintw(game->window, ++start_line, start_column, " / ____/ __ \\|  \\/  |  __ \\| |    |  ____|__   __|  ____|");
    mvwprintw(game->window, ++start_line, start_column, "| |   | |  | | \\  / | |__) | |    | |__     | |  | |__   ");
    mvwprintw(game->window, ++start_line, start_column, "| |   | |  | | |\\/| |  ___/| |    |  __|    | |  |  __|  ");
    mvwprintw(game->window, ++start_line, start_column, "| |___| |__| | |  | | |    | |____| |____   | |  | |____ ");
    mvwprintw(game->window, ++start_line, start_column, " \\_____\\____/|_|  |_|_|    |______|______|  |_|  |______|");

    wrefresh(game->window);

    delay_output(3000);
}