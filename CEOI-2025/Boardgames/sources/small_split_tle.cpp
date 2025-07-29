#include <vector>
#include <algorithm>
#include <iostream>

std::vector<std::vector<int>> g;
std::vector<bool> vis;
std::vector<int> ans, group;

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

void dfs2(std::vector<int> &q, int &cap, int x, int l, int r)
{
    if (cap == 0)
        return;
    q.push_back(x);
    vis[x] = true;
    for (int y : g[x])
    {
        --cap;
        if (!vis[y] && y >= l && y < r)
        {
            dfs2(q, cap, y, l, r);
        }
        if (cap == 0)
            break;
    }
}

void partition(int l, int r)
{
    std::fill(group.begin() + l, group.begin() + r, -1);
    std::fill(vis.begin() + l, vis.begin() + r, false);
    for (int i = 0; i < (r - l); i++)
    {
        int cap = 100;
        std::vector<int> q;
        if (i % 2 == 0)
        {
            dfs2(q, cap, l + i / 2, l, r);
        }
        else
        {
            dfs2(q, cap, r - i / 2 - 1, l, r);
        }
        std::sort(q.begin(), q.end());
        for (int x : q)
        {
            vis[x] = false;
        }
        if (cap > 0)
        {
            if ((int)q.size() == r - l)
            {
                ans.push_back(r - l);
                return;
            }
            int last = l;
            for (int j = 0; j < (int)q.size(); j++)
            {
                if (j == 0 || q[j] != q[j - 1] + 1)
                {
                    if (last != q[j])
                        partition(last, q[j]);
                    last = q[j];
                }
                if (j == (int)q.size() - 1 || q[j] + 1 != q[j + 1])
                {
                    partition(last, q[j] + 1);
                    last = q[j] + 1;
                }
            }
            if (last < r)
            {
                partition(last, r);
            }
            return;
        }
    }
    int cnt = 0;
    for (int i = l; i < r; i++)
    {
        if (group[i] == -1)
        {
            dfs(i, i, l, r);
            cnt++;
        }
    }
    if (cnt == 1)
    {
        ans.push_back(r - l);
        return;
    }
    int last = l;
    for (int i = l + 1; i < r; i++)
    {
        if (group[i] != group[i - 1])
        {
            partition(last, i);
            last = i;
        }
    }
    partition(last, r);
    return;
}

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    g.resize(N);
    vis.assign(N, false);
    group.assign(N, -1);
    for (int i = 0; i < M; i++)
    {
        g[X[i]].push_back(Y[i]);
        g[Y[i]].push_back(X[i]);
    }
    partition(0, N);
    return ans;
}