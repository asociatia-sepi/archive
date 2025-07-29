#include "boardgames.h"

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    std::vector<int> ans;
    ans.clear();

    std::vector<bool> con(N);
    for (int i = 0; i < N; i++)
        con[i] = false;

    for (int i = 0; i < M; i++)
        con[std::min(X[i], Y[i])] = true;

    int cnt = 1;
    for (int i = 0; i < N; i++)
    {
        if (con[i])
        {
            cnt++;
        }
        else
        {
            ans.push_back(cnt);
            cnt = 1;
        }
    }
    return ans;
}
