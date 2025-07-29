#include <vector>

std::vector<std::vector<int>> g;
std::vector<bool> vis;

void dfs(int x)
{
    vis[x] = true;
    for (int y : g[x])
    {
        if (!vis[y])
        {
            dfs(y);
        }
    }
}

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    std::vector<int> res;
    g.resize(N);
    vis.assign(N, false);
    for (int i = 0; i < M; i++)
    {
        g[X[i]].push_back(Y[i]);
        g[Y[i]].push_back(X[i]);
    }
    dfs(0);
    int last = 0;
    for (int i = 1; i < N; i++)
    {
        if (!vis[i])
        {
            res.push_back(i - last);
            last = i;
            std::fill(vis.begin() + i, vis.end(), false);
            dfs(i);
        }
    }
    res.push_back(N - last);
    return res;
}