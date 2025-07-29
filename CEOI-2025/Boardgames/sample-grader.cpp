#include "boardgames.h"

#include <cassert>
#include <cstdio>
#include <vector>

int main()
{
    int N, M;
    assert(2 == scanf("%d %d", &N, &M));

    std::vector<int> X(M), Y(M);

    for (int i = 0; i < M; i++)
    {
        assert(2 == scanf("%d %d", &X[i], &Y[i]));
    }
    fclose(stdin);

    std::vector<int> K = partition_players(N, M, X, Y);

    int g = static_cast<int>(K.size());
    printf("%d\n", g);
    for (int i = 0; i < g; i++)
    {
        printf(i == 0 ? "%d" : " %d", K[i]);
    }
    printf("\n");
    fclose(stdout);
    return 0;
}
