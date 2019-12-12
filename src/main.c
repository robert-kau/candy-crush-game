#include <ncurses.h>
#include <stdlib.h>
#include "inc/window.h"
#include "inc/game_structs.h"
#include "inc/game_controll.h"
#include <string.h>

int main()
{
  //alocacao de memoria para as structs do programa e atribuicao aos respectivos ponteiros
  GAME *game = malloc(sizeof(GAME));
  PLAYER *player = malloc(sizeof(PLAYER));
  LEVEL_INFO *level_info = malloc(sizeof(LEVEL_INFO));

  int exit_game = 0;

  InitWindow();

  GameInit(game);

  while (!(exit_game)) //loop geral de execucao do programa
    exit_game = GameScreenControll(game, player, level_info);

  //ao fim do programa, liberação da memoria reservada para as structs no inicio do programa
  free(game);
  free(player);
  free(level_info);

  return 0;
}