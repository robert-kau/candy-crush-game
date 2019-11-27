#define MAX_LENGTH_PLAYER_NAME 30

typedef struct Game
{
    WINDOW *window;
    char state_screen;
} GAME;

typedef struct Player
{
    char name[MAX_LENGTH_PLAYER_NAME];
    int score;
    int level;
} PLAYER;
