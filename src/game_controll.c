#include <ncurses.h>
#include "inc/game_menu.h"
#include "inc/enums.h"
#include "inc/game_init.h"

int a = 0;

void GameInit(Game *game)
{
    game->state_screen = SCREEN_MENU;
}

int GameScreenControll(Game *game)
{
    int exit_game = 0;

    switch (game->state_screen)
    {
    case SCREEN_MENU:
        MenuScreen(game);
        break;

    case SCREEN_NOVO_JOGO:
        NewGameInit(game);
        if (!a)
        {
            printf("\n\nOpcao Novo jogo\n\n");
            a = 1;
        }
        break;

    case SCREEN_CONTINUAR:
        if (!a)
        {
            printf("\n\nOpcao continuar\n\n");
            a = 1;
        }

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
