#define MAX_LENGTH_PLAYER_NAME 30
#define MAP_LINES 7
#define MAP_COL 11

typedef struct Game
{
    WINDOW *window;
    WINDOW *matrix;
    char state_screen;
} GAME;

typedef struct Player
{
    char name[MAX_LENGTH_PLAYER_NAME];
    int score;
    int level;
} PLAYER;

typedef struct Level_Info
{
    int combinations_next_level;
    int lin;
    int col;
    int first_select_col;
    int first_select_lin;
    int lin_old;
    int col_old;
    int ch;
    char tabuleiro[MAP_LINES][MAP_COL];
    int n_space;
    int n_combintions;
} LEVEL_INFO;