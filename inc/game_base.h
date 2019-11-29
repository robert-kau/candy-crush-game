#define MAP_LINES 7
#define MAP_COL 11
#define EDGE 1

#define INITIAL_LEVEL 0
#define INITIAL_SCORE 0
#define MAX_LENGTH_PLAYER_NAME 30

#define RED 1
#define GREEN 2
#define CYAN 6
#define BLUE 4
#define YELLOW 3
#define WHITE 7

#define POS_TAB_LIN 10
#define POS_TAB_COL 75

#define SPACE_SIZE 2
#define PIXELS_PIECE 16

#define INIT_POS_CURSOR 1

#define ESC 27

void NewGameInit(GAME *game, PLAYER *player);

void GameRunning(GAME *game, PLAYER *player, LEVEL_INFO *level_info);

void ReadFileLevel(PLAYER *player, char tabuleiro[][MAP_COL], LEVEL_INFO *level_info);

void PrintColorMatrix(GAME *game, char tabuleiro[][MAP_COL]);

void PrintCursorMatrix(GAME *game, int lin, int col);