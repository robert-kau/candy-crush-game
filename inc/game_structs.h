typedef struct Game
{
    WINDOW *window;
    char state_screen;
} GAME;

typedef struct Player
{
    char name[30];
    int score;
    int level;
} PLAYER;
