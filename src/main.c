#include <ncurses.h>
#include <stdlib.h>
#include "inc/window.h"
#include "inc/game_structs.h"
#include "inc/game_controll.h"

int main()
{
  InitWindow();

  Game *game = malloc(sizeof(Game));

  GameInit(game);

  while (1)
  {
    GameScreenControll(game);
  }

  return 0;
}