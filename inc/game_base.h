#define MAP_LINES 7
#define MAP_COL 12

#define INITIAL_LEVEL 0
#define INITIAL_SCORE 0
#define MAX_LENGTH_PLAYER_NAME 30

#define RED 1
#define GREEN 2
#define CYAN 6
#define BLUE 4
#define YELLOW 3

#define POS_TAB_LIN 15
#define POS_TAB_COL 75

#define SPACE_SIZE 2
#define PIXELS_PIECE 9

PLAYER *NewGameInit(GAME *game);

void GameRunning(GAME *game, PLAYER *player);

void ReadFileLevel(PLAYER *player, char tabuleiro[][MAP_COL]);

void PrintColorMatrix(GAME *game, char tabuleiro[][MAP_COL]);