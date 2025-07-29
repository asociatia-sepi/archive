#include <vector>

std::vector<std::vector<int>> g;
std::vector<int> group, ans;

void dfs(int x, int c, int l, int r)
{
    group[x] = c;
    for (int y : g[x])
    {
        if (group[y] == -1 && y >= l && y < r)
        {
            dfs(y, c, l, r);
        }
    }
}

void partition(int l, int r)
{
    std::fill(group.begin() + l, group.begin() + r, -1);
    for (int i = l; i < r; i++)
    {
        if (group[i] == -1)
        {
            dfs(i, i, l, r);
        }
    }
    int last = l;
    for (int i = l + 1; i < r; i++)
    {
        if (group[i] != group[last])
        {
            partition(last, i);
            last = i;
        }
    }
    if (last == l)
    {
        ans.push_back(r - l);
    }
    else
    {
        partition(last, r);
    }
    return;
}

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    g.resize(N);
    group.assign(N, false);
    for (int i = 0; i < M; i++)
    {
        g[X[i]].push_back(Y[i]);
        g[Y[i]].push_back(X[i]);
    }
    partition(0, N);
    return ans;
}