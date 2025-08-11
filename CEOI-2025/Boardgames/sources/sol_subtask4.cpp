#include <vector>
#include <algorithm>
#include <iostream>

struct DSU
{
    std::vector<int> p, h;
    void init(int n)
    {
        p.assign(n, -1);
        h.assign(n, 0);
    }
    int get(int x) { return p[x] == -1 ? x : p[x] = get(p[x]); }
    bool unite(int x, int y)
    {
        x = get(x), y = get(y);
        if (x == y)
            return false;
        if (h[x] < h[y])
            std::swap(x, y);
        p[y] = x;
        if (h[x] == h[y])
            h[x]++;
        return true;
    }
};

std::vector<std::vector<int>> g;
DSU dsu;

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
    std::vector<std::pair<int, int>> edges;
    for (int i = N - 1; i >= 0; i--)
    {
        dsu.init(N);
        std::vector<int> cnt(N, 0);
        for (int x : g[i])
        {
            if (x > i)
            {
                edges.push_back({i, x});
            }
        }
        std::sort(edges.begin(), edges.end(), [](std::pair<int, int> x, std::pair<int, int> y) -> bool
                  { return x.second < y.second; });

        std::vector<std::pair<int, int>> edges2;
        for (std::pair<int, int> e : edges)
        {
            if (dsu.unite(e.first, e.second))
            {
                edges2.push_back(e);
                cnt[e.second]++;
            }
        }
        std::swap(edges, edges2);
        int sum = 0;
        for (int j = i; j < N; j++)
        {
            sum += cnt[j];
            if (sum == j - i)
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