#include <ncurses.h>
#include "inc/game_menu.h"
#include "inc/enums.h"
#include "inc/game_base.h"
#include "inc/game_controll.h"
#include "inc/window.h"
#include "inc/ranking.h"
#include <string.h>

void InitRanking(GAME *game)
{
    int i = 0;

    for (i = 0; i < 10; i++)
    {
        strcpy(game->ranking[i].name, "None");
        game->ranking[i].score = 0;
    }

    SaveRankingFile(game);
    ReadRankingFile(game);
}

FileState ReadRankingFile(GAME *game)
{
    FILE *fp = fopen("/home/robert/Documents/UFRGS/Semestre_2/Algoritmos_e_Prog/trabalho_final/candy-crush-game/files_game/ranking.bin", "rb");

    if (fp == NULL)
    {
        fp = fopen("/home/robert/Documents/UFRGS/Semestre_2/Algoritmos_e_Prog/trabalho_final/candy-crush-game/files_game/ranking.bin", "wb");
        if (fp == NULL)
        {
            return CREATED_FILE_FAILL;
        }
        fclose(fp);
        return CREATED_FILE;
    }

    if (!fread(game->ranking, sizeof(PLAYER), 10, fp))
        return READING_ERROR;

    fclose(fp);

    return EVERYTHING_OK;
}

FileState SaveRankingFile(GAME *game)
{
    FILE *fp = fopen("/home/robert/Documents/UFRGS/Semestre_2/Algoritmos_e_Prog/trabalho_final/candy-crush-game/files_game/ranking.bin", "wb");

    if (fp == NULL)
        return OPENING_ERROR;

    //game->ranking[3].score = 6;

    if (!fwrite(game->ranking, sizeof(PLAYER), 10, fp))
        return READING_ERROR;

    fclose(fp);

    return EVERYTHING_OK;
}

void PrintRankingFile(GAME *game)
{
    int line = 15, col = 70, i = 0;
    int ch;

    // werase(game->window);
    // wrefresh(game->window);

    game->window = CreateNewWindow(54, 180, 15, 1);

    mvwprintw(game->window, line, col, "%s", "Nome  ------  Score");
    line++;

    for (i = 0; i < 10; i++, line++)
    {
        mvwprintw(game->window, line, col, "%s  ------  %d", game->ranking[i].name, game->ranking[i].score);
    }

    while ((ch = wgetch(game->window)) != 10)
        ;
}

void AddPlayerRanking(PLAYER *player, LEVEL_INFO *level_info, GAME *game)
{
    int i = 0;

    RankingSort(game);

    while (strcmp(game->ranking[i].name, "None") && i < MAX_PLAYER_RANKING) //se nao forem iguais
        i++;

    //i--;

    //i = 8;

    if (player->score > game->ranking[i].score) //se a pontuação do jogador é maior que a menor pontuação do ranking, substitui
    {
        game->ranking[i].score = player->score;
        strcpy(game->ranking[i].name, player->name);

        RankingSort(game);

        SaveRankingFile(game);
    }
}

void RankingSort(GAME *game)
{
    int i, j;
    int temp;
    char temp2[MAX_LENGTH_PLAYER_NAME];

    for (i = MAX_PLAYER_RANKING - 1; i >= 0; i--)
        //for (i = 0; i < MAX_PLAYER_RANKING - 1; i++)
        for (j = 0; j <= MAX_PLAYER_RANKING - 1; j++)
            if (game->ranking[j].score < game->ranking[j + 1].score)
            {
                temp = game->ranking[j].score;
                strcpy(temp2, game->ranking[j].name);

                game->ranking[j].score = game->ranking[j + 1].score;
                strcpy(game->ranking[j].name, game->ranking[j + 1].name);

                game->ranking[j + 1].score = temp;
                strcpy(game->ranking[j + 1].name, temp2);
            }
}
