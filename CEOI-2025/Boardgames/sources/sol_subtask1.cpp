#include <vector>

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    std::vector<int> res;
    std::vector<bool> split(N, true);
    for (int i = 0; i < M; i++)
    {
        split[Y[i]] = false;
    }
    int last = 0;
    for (int i = 1; i < N; i++)
    {
        if (split[i])
        {
            res.push_back(i - last);
            last = i;
        }
    }
    res.push_back(N - last);
    return res;
}