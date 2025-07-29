#include <vector>
#include <map>

struct DSU
{
    std::vector<int> p, h, sz;
    void init(int n)
    {
        p.assign(n, -1);
        h.assign(n, 0);
        sz.assign(n, 1);
    }
    void reset(int l, int r)
    {
        std::fill(p.begin() + l, p.begin() + r, -1);
        std::fill(h.begin() + l, h.begin() + r, 0);
        std::fill(sz.begin() + l, sz.begin() + r, 1);
    }
    int get(int x) { return p[x] == -1 ? x : p[x] = get(p[x]); }
    int getsz(int x) { return sz[get(x)]; }
    bool unite(int x, int y)
    {
        x = get(x), y = get(y);
        if (x == y)
            return false;
        if (h[x] < h[y])
            std::swap(x, y);
        p[y] = x;
        sz[x] += sz[y];
        if (h[x] == h[y])
            h[x]++;
        return true;
    }
};

std::vector<std::vector<int>> g;

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    std::vector<int> res;
    g.resize(N);
    for (int i = 0; i < M; i++)
    {
        g[X[i]].push_back(Y[i]);
        g[Y[i]].push_back(X[i]);
    }
    const int B = 2000;
    DSU dsu;
    dsu.init(N);
    int i = 0;
    while (i < N)
    {
        int cnt = 0, i2 = i + 1, j = i + 1;
        while (j < N && cnt < B)
        {
            for (int x : g[j])
            {
                if (i <= x && x < j)
                {
                    dsu.unite(j, x);
                }
            }
            cnt += std::max((int)g[j].size(), 1);
            if (dsu.getsz(i) == j - i + 1)
            {
                i2 = j + 1;
                cnt = 0;
            }
            ++j;
        }
        dsu.reset(i, j);
        res.push_back(i2 - i);
        i = i2;
    }
    return res;
}