#include <vector>

std::vector<std::vector<int>> g;
std::vector<bool> vis;

void dfs(int x, int l, int r)
{
    vis[x] = true;
    for (int y : g[x])
    {
        if (!vis[y] && y >= l && y <= r)
        {
            dfs(y, l, r);
        }
    }
}

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    std::vector<int> res;
    g.resize(N);
    for (int i = 0; i < M; i++)
    {
        g[X[i]].push_back(Y[i]);
        g[Y[i]].push_back(X[i]);
    }

    std::vector<int> step(N);
    for (int i = 0; i < N; i++)
    {
        step[i] = i + 1;
        for (int j = i + 1; j < N; j++)
        {
            vis.assign(N, false);
            dfs(i, i, j);
            bool ok = true;
            for (int k = i; k <= j; k++)
            {
                ok = ok & vis[k];
            }
            if (ok)
            {
                step[i] = j + 1;
            }
        }
    }

    int x = 0;
    while (x < N)
    {
        res.push_back(step[x] - x);
        x = step[x];
    }

    return res;
}