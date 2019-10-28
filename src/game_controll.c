#include <ncurses.h>
#include "inc/game_menu.h"
#include "inc/enums.h"

void GameInit(Game *game)
{
    game->state_screen = SCREEN_MENU;
}

void GameScreenControll(Game *game)
{
    switch (game->state_screen)
    {
    case SCREEN_MENU:
        MenuScreen(game);
        break;
    }
}
