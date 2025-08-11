#include <vector>

std::vector<std::vector<int>> g;
std::vector<bool> vis;
std::vector<int> ans;

void dfs(int x, int l, int r)
{
    vis[x] = true;
    for (int y : g[x])
    {
        if (!vis[y] && y >= l && y < r)
        {
            dfs(y, l, r);
        }
    }
}

void partition(int l, int r)
{
    std::fill(vis.begin() + l, vis.begin() + r, false);
    dfs(l, l, r);
    for (int i = l + 1; i < r; i++)
    {
        if (!vis[i])
        {
            partition(l, i);
            partition(i, r);
            return;
        }
    }
    ans.push_back(r - l);
    return;
}

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    g.resize(N);
    vis.assign(N, false);
    for (int i = 0; i < M; i++)
    {
        g[X[i]].push_back(Y[i]);
        g[Y[i]].push_back(X[i]);
    }
    partition(0, N);
    return ans;
}