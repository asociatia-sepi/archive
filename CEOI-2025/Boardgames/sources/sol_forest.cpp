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

struct segment_tree
{
    std::vector<int> st, stadd;
    int n;
    void init(int n_)
    {
        n = 1 << (32 - __builtin_clz(n_));
        st.assign(2 * n, 0);
        stadd.assign(2 * n, 0);
        std::fill(stadd.begin() + n, stadd.end(), -1e9);
        for (int i = 2 * n - 1; i > 0; i--)
            upd(i);
    }
    void upd(int x)
    {
        if (x < n)
        {
            st[x] = std::max(st[2 * x], st[2 * x + 1]) + stadd[x];
        }
        else
        {
            st[x] = stadd[x];
        }
    }

    void update(int L, int R, int c, int x, int l, int r)
    {
        if (R <= l || r <= L)
            return;
        if (L <= l && r <= R)
        {
            stadd[x] += c;
            upd(x);
            return;
        }
        int m = (l + r) / 2;
        update(L, R, c, 2 * x, l, m);
        update(L, R, c, 2 * x + 1, m, r);
        upd(x);
    }
    void update(int l, int r, int c) { update(l, r, c, 1, 0, n); }

    int query(int x)
    {
        int res = 0;
        for (int i = x + n; i > 0; i >>= 1)
        {
            res += stadd[i];
        }
        return res;
    }

    int getmax(int x = 1)
    {
        return x >= n ? x - n : (st[2 * x] > st[2 * x + 1] ? getmax(2 * x) : getmax(2 * x + 1));
    }
};

std::vector<std::vector<int>> g;
segment_tree st;
DSU dsu;

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    std::vector<int> res;
    g.resize(N);
    st.init(N);
    dsu.init(N);
    for (int i = 0; i < M; i++)
    {
        g[X[i]].push_back(Y[i]);
        g[Y[i]].push_back(X[i]);
    }

    std::vector<int> step(N);
    for (int i = N - 1; i >= 0; i--)
    {
        for (int x : g[i])
        {
            if (i < x && dsu.unite(i, x))
            {
                st.update(x, N, 1);
            }
        }
        st.update(i, i + 1, (int)1e9 - i);
        int j = st.getmax();
        step[i] = j + 1;
    }

    int x = 0;
    while (x < N)
    {
        res.push_back(step[x] - x);
        if (x > step[x])
            return {};
        x = step[x];
    }

    return res;
}