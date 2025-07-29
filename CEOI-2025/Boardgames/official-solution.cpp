#include <bits/stdc++.h>

using namespace std;

struct DSU
{
    vector<int> link, sz, stk, cnt;
    DSU(int n, int m) : link(n, -1), sz(n, 1), cnt(m, 0) {}

    int Find(int x)
    {
        while (link[x] != -1)
            x = link[x];
        return x;
    }
    void unite(int x, int y)
    {
        x = Find(x);
        y = Find(y);
        if (x == y)
            return;
        if (sz[x] > sz[y])
            swap(x, y);
        stk.push_back(x);
        link[x] = y;
        sz[y] += sz[x];
    }
    void Add(int x, int y, int i)
    {
        stk.push_back(~i);
        if (++cnt[i] == 2)
            unite(x, y);
    }
    int Ckp()
    {
        return stk.size();
    }
    void Undo(int ckp)
    {
        while (ckp != Ckp())
        {
            int x = stk.back();
            stk.pop_back();
            if (x < 0)
            {
                cnt[~x] -= 1;
            }
            else
            {
                int y = link[x];
                assert(y != -1);
                sz[y] -= sz[x];
                link[x] = -1;
            }
        }
    }
};

struct Solver
{
    int n, m, cnt;
    vector<int> start, ans;
    vector<tuple<int, int, int>> edges;
    vector<int> ckps;
    DSU dsu;

    Solver(int n, vector<pair<int, int>> &es) : n(n), m(es.size()), cnt(0),
                                                start(n + 1), ans(n),
                                                edges(2 * m), ckps(2 * m, -1), dsu(n, m)
    {
        for (int i = 0; i < m; ++i)
        {
            auto [a, b] = es[i];
            edges[2 * i] = {a, b, i};
            edges[2 * i + 1] = {b, a, i};
            start[a + 1] += 1;
            start[b + 1] += 1;
        }
        sort(edges.begin(), edges.end());
        for (int i = 0; i < n; ++i)
            start[i + 1] += start[i];
    }

    vector<int> Solve()
    {
        divide(0, n, -1);
        for (int i = 1; i < n; ++i)
            ans[i] += ans[i - 1];
        return ans;
    }

    template <class CB>
    void iter_seg(int b, int e, int emid, CB cb)
    {
        int l = emid, r = emid;
        for (int step = (1 << 20); step; step /= 2)
        {
            if (l - step >= start[b])
                cb(l - step, l), l -= step;
            if (r + step <= start[e])
                cb(r, r + step), r += step;
        }
    }
    void push(int b, int e, int emid, int skip)
    {
        iter_seg(b, e, emid, [&](int l, int r)
                 {
      if (skip-- > 0) return;
      ckps[l] = dsu.Ckp();
      for (int i = l; i < r; ++i) {
        auto [u, v, ei] = edges[i];
        dsu.Add(u, v, ei);
      } });
    }
    int pop(int b, int e, int emid, int x)
    {
        int skip = 0, is_bad = 0;
        iter_seg(b, e, emid, [&](int l, int r)
                 {
      if (start[x] >= l && start[x] < r)
        dsu.Undo(ckps[l]), is_bad = true;
      if (!is_bad) ++skip; });
        return skip;
    }
    void divide(int b, int e, int emid)
    {
        if (b >= e)
            return;
        if (emid == -1)
        { // rebuild from scratch
            dsu.Undo(0);
            emid = (start[b] + start[e]) / 2;
            push(b, e, emid, 0);
        }

        for (int l = b, r = e - 1, it = 0; it < e - b - 1; ++it)
        {
            if (it % 2 == 0)
            { // from left
                if (dsu.Find(l) == dsu.Find(l + 1))
                {
                    ++l;
                    continue;
                }
                int m = l + 1;
                if (start[m] >= emid)
                    emid = -1;
                else
                    push(m, e, emid, pop(b, e, emid, m));
                divide(m, e, emid);
                divide(b, m, -1);
                return;
            }
            else
            { // from right
                if (dsu.Find(r) == dsu.Find(r - 1))
                {
                    --r;
                    continue;
                }
                int m = r;
                if (start[m] < emid)
                    emid = -1;
                else
                    push(b, m, emid, pop(b, e, emid, m));
                divide(b, m, emid);
                divide(m, e, -1);
                return;
            }
        }
        ans[b] += 1;
    }
};

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    int n = N;
    int m = M;
    vector<pair<int, int>> es(m);
    for (int i = 0; i < m; ++i)
    {
        int a = X[i];
        int b = Y[i];
        assert(a != b);
        es[i] = {a, b};
    }
    Solver S(n, es);
    auto ans = S.Solve();
    vector<int> res(ans.back());
    for (int i = 0; i < N; i++)
        res[ans[i] - 1]++;
    return res;
}