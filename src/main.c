#include <ncurses.h>
#include <stdlib.h>
#include "inc/window.h"
#include "inc/game_structs.h"
#include "inc/game_controll.h"

int main()
{

  Game *game = malloc(sizeof(Game));
  int exit_game = 0;

  InitWindow();

  GameInit(game);

  while (!(exit_game))
  {
    exit_game = GameScreenControll(game);
  }

  return 0;
}