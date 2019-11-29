#define MAX_LENGTH_PLAYER_NAME 30

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
} LEVEL_INFO;