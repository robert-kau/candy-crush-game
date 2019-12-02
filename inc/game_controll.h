#include <ncurses.h>
//#include "inc/game_structs.h"

#define MAX_LEVELS 3

int GameScreenControll(GAME *game, PLAYER *player, LEVEL_INFO *level_info);

void GameInit(GAME *game);

void CloseGame(GAME *game, PLAYER *player, LEVEL_INFO *level_info);

void PrintScreenGameComplete(GAME *game);

void PrintScreenGameOver(GAME *game);

void PrintScreenLevelComplete(GAME *game);