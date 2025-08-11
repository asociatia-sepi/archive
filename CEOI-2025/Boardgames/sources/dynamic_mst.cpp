#include <algorithm>
#include <iostream>
#include <vector>
#include <cassert>

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

struct edge
{
    int u, v, x, y, ind;
};

edge compr_dfs(const std::vector<std::vector<edge>> &g, std::vector<bool> &vis, std::vector<bool> &used, std::vector<edge> &edges, int x)
{
    vis[x] = true;
    edge res = {-1, -1, -1, -1, -1}, up = {-1, -1, -1, -1, -1};
    for (edge e : g[x])
    {
        if (!vis[e.v])
        {
            edge tmp = compr_dfs(g, vis, used, edges, e.v);
            if (tmp.u == -1)
                continue;
            if (res.u != -1 && !used[x])
            {
                used[x] = true;
                edges[res.ind] = {res.u, x, res.x, res.y, res.ind};
                edges[tmp.ind] = {tmp.u, x, tmp.x, tmp.y, tmp.ind};
                res = {-1, -1, -1, -1, -1};
            }
            else if (used[x])
            {
                edges[tmp.ind] = {tmp.u, x, tmp.x, tmp.y, tmp.ind};
            }
            else
            {
                res = tmp;
            }
        }
        else
        {
            up = e;
        }
    }
    if (used[x])
    {
        res = up;
    }
    else if (res.u != -1 && up.y > res.y)
    {
        up.u = res.u;
        res = up;
    }
    return res;
}

std::pair<std::vector<edge>, int> compress(std::vector<edge> &edges, const std::vector<edge> &mst_edges, int n)
{
    std::vector<std::vector<edge>> g(n);
    std::vector<bool> vis(n), used(n);
    for (int i = 0; i < (int)mst_edges.size(); i++)
    {
        const edge &e = mst_edges[i];
        g[e.u].push_back({e.u, e.v, e.x, e.y, i});
        g[e.v].push_back({e.v, e.u, e.x, e.y, i});
    }
    for (edge e : edges)
    {
        used[e.u] = used[e.v] = true;
    }
    std::vector<edge> used_edges((int)mst_edges.size(), {-1, -1, -1, -1, -1});
    for (int i = 0; i < n; i++)
    {
        if (!vis[i] && used[i])
        {
            compr_dfs(g, vis, used, used_edges, i);
        }
    }
    std::vector<edge> res;
    for (int i = 0; i < (int)mst_edges.size(); i++)
    {
        edge &e = used_edges[i];
        if (e.u != -1)
        {
            res.push_back(used_edges[i]);
            res.back().ind = mst_edges[i].ind;
        }
    }
    std::vector<int> idx(n);
    int id = 0;
    for (int i = 0; i < n; i++)
    {
        if (used[i])
        {
            idx[i] = id++;
        }
    }
    for (edge &e : edges)
    {
        e.u = idx[e.u];
        e.v = idx[e.v];
    }
    for (edge &e : res)
    {
        e.u = idx[e.u];
        e.v = idx[e.v];
    }
    return {res, id};
}

std::vector<std::vector<int>> add, rem;

void precalc_mst(const std::vector<edge> &edges, const std::vector<edge> &mst_edges, int l, int r, int n)
{
    if (l + 1 == r)
    {
        DSU dsu;
        dsu.init(n);
        int j1 = 0, j2 = 0;
        while (j1 < (int)edges.size() || j2 < (int)mst_edges.size())
        {
            if (j2 == (int)mst_edges.size() || (j1 < (int)edges.size() && edges[j1].y < mst_edges[j2].y))
            {
                if (dsu.unite(edges[j1].u, edges[j1].v))
                {
                    add[l].push_back(edges[j1].ind);
                }
                ++j1;
            }
            else
            {
                if (!dsu.unite(mst_edges[j2].u, mst_edges[j2].v))
                {
                    rem[l].push_back(mst_edges[j2].ind);
                }
                ++j2;
            }
        }
        return;
    }
    int mid = (l + r) / 2;
    std::vector<edge> edgesL, edgesR;
    for (edge e : edges)
    {
        if (e.x < mid)
        {
            edgesL.push_back(e);
        }
        else
        {
            edgesR.push_back(e);
        }
    }
    std::vector<edge> mst_edges2;
    DSU dsu;
    dsu.init(n);
    int j1 = 0, j2 = 0;
    while (j1 < (int)edgesR.size() || j2 < (int)mst_edges.size())
    {
        if (j2 == (int)mst_edges.size() || (j1 < (int)edgesR.size() && edgesR[j1].y < mst_edges[j2].y))
        {
            if (dsu.unite(edgesR[j1].u, edgesR[j1].v))
            {
                mst_edges2.push_back(edgesR[j1]);
            }
            ++j1;
        }
        else
        {
            if (dsu.unite(mst_edges[j2].u, mst_edges[j2].v))
            {
                mst_edges2.push_back(mst_edges[j2]);
            }
            ++j2;
        }
    }
    auto cr = compress(edgesR, mst_edges, n);
    auto cl = compress(edgesL, mst_edges2, n);
    precalc_mst(edgesR, cr.first, mid, r, cr.second);
    precalc_mst(edgesL, cl.first, l, mid, cl.second);
}

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

    std::vector<edge> edges(M);
    for (int i = 0; i < M; i++)
    {
        edges[i] = {X[i], Y[i], X[i], Y[i], i};
    }
    std::sort(edges.begin(), edges.end(), [](auto a, auto b) -> bool
              { return a.y < b.y; });

    add.resize(N);
    rem.resize(N);
    precalc_mst(edges, {}, 0, N, N);

    std::vector<int> step(N);
    for (int i = N - 1; i >= 0; i--)
    {
        for (int j : add[i])
        {
            st.update(Y[j], N, 1);
        }
        for (int j : rem[i])
        {
            st.update(Y[j], N, -1);
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