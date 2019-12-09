#include <ncurses.h>
#include <stdlib.h>
#include "inc/window.h"
#include "inc/game_structs.h"
#include "inc/game_controll.h"
#include <string.h>

int main()
{

  GAME *game = malloc(sizeof(GAME));
  PLAYER *player = malloc(sizeof(PLAYER));
  LEVEL_INFO *level_info = malloc(sizeof(LEVEL_INFO));

  int exit_game = 0;

  InitWindow();

  GameInit(game);

  while (!(exit_game))
  {
    exit_game = GameScreenControll(game, player, level_info);
  }

  free(game);
  free(player);
  free(level_info);

  return 0;
}