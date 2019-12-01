#include <ncurses.h>
#include "inc/game_menu.h"
#include "inc/enums.h"
#include "inc/game_base.h"

int a = 0;

void GameInit(GAME *game)
{
    game->state_screen = SCREEN_MENU;
}

int GameScreenControll(GAME *game, PLAYER *player, LEVEL_INFO *level_info)
{
    int exit_game = 0;

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

        GameRunning(game, player, level_info);

        game->state_screen = SCREEN_MENU;
        break;

    case SCREEN_FECHAR_JOGO:
        if (!a)
        {
            printf("\n\nOpcao Fechar jogo\n\n");
            a = 1;
        }
        exit_game = 1;
        break;
    }
    return exit_game;
}
