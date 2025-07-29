#include <vector>

std::vector<std::vector<int>> g;
std::vector<int> group;

void dfs(int x, int c)
{
    group[x] = c;
    for (int y : g[x])
    {
        if (group[y] == -1)
        {
            dfs(y, c);
        }
    }
}

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    std::vector<int> res;
    g.resize(N);
    group.assign(N, -1);
    for (int i = 0; i < M; i++)
    {
        g[X[i]].push_back(Y[i]);
        g[Y[i]].push_back(X[i]);
    }
    for (int i = 0; i < N; i++)
    {
        if (group[i] == -1)
        {
            dfs(i, i);
        }
    }
    int last = 0;
    for (int i = 1; i < N; i++)
    {
        if (group[i] != group[i - 1])
        {
            res.push_back(i - last);
            last = i;
        }
    }
    res.push_back(N - last);
    return res;
}