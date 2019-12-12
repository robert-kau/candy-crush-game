#include <ncurses.h>
#include "inc/game_menu.h"
#include "inc/enums.h"
#include "inc/game_base.h"
#include "inc/game_controll.h"
#include "inc/window.h"
#include "inc/ranking.h"

#define ENTER 10

int a = 0;

int GameScreenControll(GAME *game, PLAYER *player, LEVEL_INFO *level_info)
{
    int exit_game = 0, state_game, save_game;

    switch (game->state_screen) //maquina de estados geral do jogo
    {
    case SCREEN_MENU:
        MenuScreen(game);
        break;

    case SCREEN_NOVO_JOGO:
        NewGameInit(game, player, level_info);
        game->state_screen = SCREEN_RUNNING;
        break;

    case SCREEN_CONTINUAR:
        if (level_info->time_start != 0) //se ja tiver um jogo em andamento
            game->state_screen = SCREEN_RUNNING;
        else if (ReadSavedGame(game, player, level_info))
            game->state_screen = SCREEN_RUNNING;
        else
            game->state_screen = SCREEN_MENU; //se nao existir arquivo de jogo salvo nem jogo em andamento, volta para menu
        break;

    case SCREEN_RANKING:
        if (ReadRankingFile(game) == CREATED_FILE)
            InitRanking(game);
        PrintRankingFile(game);
        game->state_screen = SCREEN_MENU;
        break;

    case SCREEN_COMO_JOGAR:
        PrintScreenHowtoPlay(game);
        game->state_screen = SCREEN_MENU;
        break;

    case SCREEN_RUNNING:
        state_game = GameRunning(game, player, level_info);

        if (state_game == GAME_PAUSE)
            game->state_screen = SCREEN_MENU;
        else if (state_game == GAME_LEVEL_FINISHED)
        {
            game->state_screen = SCREEN_LEVEL_FINISHED;
        }
        else if (state_game == GAME_TIMEOUT)
        {
            game->state_screen = SCREEN_GAME_OVER;
        }
        break;

    case SCREEN_LEVEL_FINISHED:
        player->score += CalculateScore(level_info, player);
        AddPlayerRanking(player, game);

        if (player->level < MAX_LEVELS - 1)
        {
            PrintScreenLevelComplete(game);
            player->level++;
            NewGameInit(game, player, level_info);
            game->state_screen = SCREEN_RUNNING;
        }

        else
            game->state_screen = SCREEN_WINNER;

        break;

    case SCREEN_WINNER:
        PrintScreenGameComplete(game);
        game->state_screen = SCREEN_MENU;
        break;

    case SCREEN_GAME_OVER:
        AddPlayerRanking(player, game);
        PrintScreenGameOver(game);
        game->state_screen = SCREEN_MENU;
        break;

    case SCREEN_FECHAR_JOGO:
        save_game = CloseGame(game);
        if (save_game) //se o jogador responder que quer salvar o jogo, ele deve ser salvo
            SaveGame(game, player, level_info);
        exit_game = 1;
        break;
    }
    return exit_game;
}

void GameInit(GAME *game) // inicia jogo na tela de menu
{
    game->state_screen = SCREEN_MENU;
}

int CloseGame(GAME *game)
{
    int ch;
    int i = 23;

    char menu_list[2][4] = {"SIM", "NAO"};

    game->window = CreateNewWindow(12, 60, 70, 1);

    mvwprintw(game->window, 2, 18, "%s", "Voce deseja salvar o jogo?");

    wattron(game->window, A_STANDOUT);
    mvwprintw(game->window, 6, 23, "%s", menu_list[0]);

    wattroff(game->window, A_STANDOUT);

    mvwprintw(game->window, 6, 34, "%s", menu_list[1]);

    wrefresh(game->window);

    nodelay(game->window, FALSE);

    while ((ch = wgetch(game->window)) != ENTER)
    {
        mvwprintw(game->window, 6, i, "%s", menu_list[(i % 23) ? 1 : 0]);

        switch (ch)
        {
        case KEY_LEFT:
        case 'a':
            i = 23;
            break;
        case KEY_RIGHT:
        case 'd':
            i = 34;
            break;
        }

        wattron(game->window, A_STANDOUT);

        mvwprintw(game->window, 6, i, "%s", menu_list[(i % 23) ? 1 : 0]);
        wattroff(game->window, A_STANDOUT);
    }
    endwin();

    return (i % 23) ? 0 : 1;
}

void SaveGame(GAME *game, PLAYER *player, LEVEL_INFO *level_info)
{
    FILE *arq;

    arq = fopen("/home/robert/Documents/UFRGS/Semestre_2/Algoritmos_e_Prog/trabalho_final/candy-crush-game/files_game/saved_game.bin", "wb");

    fwrite(level_info, sizeof(LEVEL_INFO), 1, arq);

    fwrite(player, sizeof(PLAYER), 1, arq);

    fwrite(game, sizeof(GAME), 1, arq);

    fclose(arq);
}

int ReadSavedGame(GAME *game, PLAYER *player, LEVEL_INFO *level_info)
{
    FILE *arq;
    int state;

    arq = fopen("/home/robert/Documents/UFRGS/Semestre_2/Algoritmos_e_Prog/trabalho_final/candy-crush-game/files_game/saved_game.bin", "rb");

    if (arq == NULL)
        state = 0;
    else
    {
        fread(level_info, sizeof(LEVEL_INFO), 1, arq);

        fread(player, sizeof(PLAYER), 1, arq);

        fread(game, sizeof(GAME), 1, arq);

        fclose(arq);

        state = 1;
    }

    return state;
}

void PrintScreenGameComplete(GAME *game)
{
    werase(game->window);
    wrefresh(game->window);

    game->window = CreateNewWindow(54, 180, 15, 1);

    int start_line = 15;
    int start_column = 70;

    mvwprintw(game->window, start_line, start_column, "               _____          __  __ ______              ");
    mvwprintw(game->window, ++start_line, start_column, "              / ____|   /\\   |  \\/  |  ____|             ");
    mvwprintw(game->window, ++start_line, start_column, "             | |  __   /  \\  | \\  / | |__                ");
    mvwprintw(game->window, ++start_line, start_column, "             | | |_ | / /\\ \\ | |\\/| |  __|               ");
    mvwprintw(game->window, ++start_line, start_column, "             | |__| |/ ____ \\| |  | | |____              ");
    mvwprintw(game->window, ++start_line, start_column, "  _____ ____  \\_____/_/___ \\_\\_|  |_|______|_____ ______ ");
    mvwprintw(game->window, ++start_line, start_column, " / ____/ __ \\|  \\/  |  __ \\| |    |  ____|__   __|  ____|");
    mvwprintw(game->window, ++start_line, start_column, "| |   | |  | | \\  / | |__) | |    | |__     | |  | |__   ");
    mvwprintw(game->window, ++start_line, start_column, "| |   | |  | | |\\/| |  ___/| |    |  __|    | |  |  __|  ");
    mvwprintw(game->window, ++start_line, start_column, "| |___| |__| | |  | | |    | |____| |____   | |  | |____ ");
    mvwprintw(game->window, ++start_line, start_column, " \\_____\\____/|_|  |_|_|    |______|______|  |_|  |______|");

    wrefresh(game->window);

    delay_output(3000);
}

void PrintScreenGameOver(GAME *game)
{
    game->window = CreateNewWindow(54, 180, 15, 1);

    int start_line = 15;
    int start_column = 70;

    mvwprintw(game->window, start_line, start_column, "  _____          __  __ ______    ______      ________ _____  _ ");
    mvwprintw(game->window, ++start_line, start_column, " / ____|   /\\   |  \\/  |  ____|  / __ \\ \\    / /  ____|  __ \\| |");
    mvwprintw(game->window, ++start_line, start_column, "| |  __   /  \\  | \\  / | |__    | |  | \\ \\  / /| |__  | |__) | |");
    mvwprintw(game->window, ++start_line, start_column, "| | |_ | / /\\ \\ | |\\/| |  __|   | |  | |\\ \\/ / |  __| |  _  /| |");
    mvwprintw(game->window, ++start_line, start_column, "| |__| |/ ____ \\| |  | | |____  | |__| | \\  /  | |____| | \\ \\|_|");
    mvwprintw(game->window, ++start_line, start_column, " \\_____/_/    \\_\\_|  |_|______|  \\____/   \\/   |______|_|  \\_(_)");

    wrefresh(game->window);

    delay_output(3000);
}

void PrintScreenLevelComplete(GAME *game)
{
    werase(game->window);
    wrefresh(game->window);

    game->window = CreateNewWindow(54, 180, 15, 1);

    int start_line = 15;
    int start_column = 70;

    mvwprintw(game->window, ++start_line, start_column, "              _        ______  __          __  ______   _                         ");
    mvwprintw(game->window, ++start_line, start_column, "             | |      |  ____| \\ \\      / / |  ____| | |                       ");
    mvwprintw(game->window, ++start_line, start_column, "             | |      |  |___   \\ \\    / /  |  |___  | |                             ");
    mvwprintw(game->window, ++start_line, start_column, "             | |      |   ___|   \\ \\  / /   |   ___| | |                           ");
    mvwprintw(game->window, ++start_line, start_column, "             | |___   |  |___     \\ \\/ /    |  |___  | |____      ");
    mvwprintw(game->window, ++start_line, start_column, "  _____ ____ |______| |______|     \\__ /     |______| |______|");
    mvwprintw(game->window, ++start_line, start_column, " / ____/ __ \\|  \\/  |  __ \\| |    |  ____|__   __|  ____|");
    mvwprintw(game->window, ++start_line, start_column, "| |   | |  | | \\  / | |__) | |    | |__     | |  | |__   ");
    mvwprintw(game->window, ++start_line, start_column, "| |   | |  | | |\\/| |  ___/| |    |  __|    | |  |  __|  ");
    mvwprintw(game->window, ++start_line, start_column, "| |___| |__| | |  | | |    | |____| |____   | |  | |____ ");
    mvwprintw(game->window, ++start_line, start_column, " \\_____\\____/|_|  |_|_|    |______|______|  |_|  |______|");

    wrefresh(game->window);

    delay_output(3000);
}

void PrintScreenHowtoPlay(GAME *game)
{
    werase(game->window);
    wrefresh(game->window);

    game->window = CreateNewWindow(54, 180, 15, 1);

    int start_line = 15;
    int start_column = 40;
    int ch;

    mvwprintw(game->window, ++start_line, start_column, "O objetivo do jogo eh combinar verticalmente 3 pecas em uma coluna ou horizontalmente 3 pecas em uma linha.");
    mvwprintw(game->window, ++start_line, start_column, "O jogador deve trocar a posicao entre duas pecas para que se forme uma combinacao.");
    mvwprintw(game->window, ++start_line, start_column, "Ao se formar uma combinacao, ela eh eliminada da tela, atribuindo uma pontuacao para o jogador.");
    mvwprintw(game->window, ++start_line, start_column, "O jogo eh estruturado em niveis.");
    mvwprintw(game->window, ++start_line, start_column, "Dependendo do nivel, o jogador tem um limite de jogadas que ele pode fazer ou um determinado tempo");
    mvwprintw(game->window, ++start_line, start_column, "para fazer um certo numero de pontos para poder passar para o proximo nivel, e essa dificuldade aumenta gradativamente.");
    mvwprintw(game->window, ++start_line, start_column, "Durante cada nivel, o jogador obtem um score calculado com base no tempo gasto para terminar cada nivel.");
    mvwprintw(game->window, ++start_line, start_column, "Para mover o cursor, o jogador pode usar as setas ou as teclas w, s, a, d do teclado.");
    mvwprintw(game->window, ++start_line, start_column, "Para selecionar a peca, o jogador deve usar a tecla espaco.");

    wrefresh(game->window);

    while ((ch = wgetch(game->window)) != 10)
        ;
}
