#define MAX_PLAYER_RANKING 10

void InitRanking(GAME *game);

FileState ReadRankingFile(GAME *game);

void PrintRankingFile(GAME *game);

FileState SaveRankingFile(GAME *game);

void AddPlayerRanking(PLAYER *player, GAME *game);

void RankingSort(GAME *game);